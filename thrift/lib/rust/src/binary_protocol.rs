/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use crate::bufext::BufMutExt;
use crate::deserialize::Deserialize;
use crate::errors::ProtocolError;
use crate::framing::Framing;
use crate::protocol::{Protocol, ProtocolReader, ProtocolWriter};
use crate::serialize::Serialize;
use crate::thrift_protocol::{MessageType, ProtocolID};
use crate::ttype::TType;
use crate::Result;
use bufsize::SizeCounter;
use bytes::{Buf, Bytes, BytesMut};
use ghost::phantom;
use std::{cmp, convert::TryFrom, io::Cursor};

pub const BINARY_VERSION_MASK: u32 = 0xffff_0000;
pub const BINARY_VERSION_1: u32 = 0x8001_0000;

/// A straight-forward binary format that encodes numeric values in fixed width.
///
/// ```ignore
/// let protocol = BinaryProtocol;
/// let transport = HttpClient::new(ENDPOINT)?;
/// let client = BuckGraphService::new(protocol, transport);
/// ```
///
/// The type parameter is the Framing expected by the transport on which this
/// protocol is operating. Usually by convention the transport itself serves as
/// the Framing impl, so for example in the case of HttpClient above, the
/// compiler has inferred `F = HttpClient`.
///
/// Where the compiler reports that a Framing can't be inferred, one can be
/// specified explicitly:
///
/// ```ignore
/// let protocol = BinaryProtocol::<SRHeaderTransport>;
/// ```
#[phantom]
#[derive(Copy, Clone)]
pub struct BinaryProtocol<F = Bytes>;

pub struct BinaryProtocolSerializer<B> {
    buffer: B,
}

pub struct BinaryProtocolDeserializer<B> {
    buffer: B,
}

impl<F> Protocol for BinaryProtocol<F>
where
    F: Framing + 'static,
{
    type Frame = F;
    type Sizer = BinaryProtocolSerializer<SizeCounter>;
    type Serializer = BinaryProtocolSerializer<F::EncBuf>;
    type Deserializer = BinaryProtocolDeserializer<F::DecBuf>;

    const PROTOCOL_ID: ProtocolID = ProtocolID::BinaryProtocol;

    fn serializer<SZ, SER>(size: SZ, ser: SER) -> <Self::Serializer as ProtocolWriter>::Final
    where
        SZ: FnOnce(&mut Self::Sizer),
        SER: FnOnce(&mut Self::Serializer),
    {
        let mut sizer = BinaryProtocolSerializer {
            buffer: SizeCounter::new(),
        };
        size(&mut sizer);
        let sz = sizer.finish();
        let mut buf = BinaryProtocolSerializer {
            buffer: F::enc_with_capacity(sz),
        };
        ser(&mut buf);
        buf.finish()
    }

    fn deserializer(buf: F::DecBuf) -> Self::Deserializer {
        BinaryProtocolDeserializer::new(buf)
    }

    fn into_buffer(deser: Self::Deserializer) -> F::DecBuf {
        deser.into_inner()
    }
}

impl<B> BinaryProtocolSerializer<B> {
    pub fn with_buffer(buffer: B) -> Self {
        Self { buffer }
    }
}

impl<B: BufMutExt> BinaryProtocolSerializer<B> {
    fn write_u32(&mut self, value: u32) {
        self.buffer.put_u32(value)
    }
}

impl<B: Buf> BinaryProtocolDeserializer<B> {
    pub fn new(buffer: B) -> Self {
        BinaryProtocolDeserializer { buffer }
    }

    pub fn into_inner(self) -> B {
        self.buffer
    }

    fn peek_bytes(&self, len: usize) -> Option<&[u8]> {
        if self.buffer.bytes().len() >= len {
            Some(&self.buffer.bytes()[..len])
        } else {
            None
        }
    }

    fn read_bytes_into<'a>(&mut self, len: usize, result: &'a mut Vec<u8>) -> Result<()> {
        ensure_err!(
            self.buffer.remaining() >= len,
            ProtocolError::InvalidDataLength
        );

        let mut remaining = len;

        while remaining > 0 {
            let length = {
                let buffer = self.buffer.bytes();
                let length = cmp::min(remaining, buffer.len());
                result.extend_from_slice(&buffer[..length]);
                length
            };

            remaining -= length;
            self.buffer.advance(length);
        }

        Ok(())
    }

    fn read_u32(&mut self) -> Result<u32> {
        ensure_err!(self.buffer.remaining() >= 4, ProtocolError::EOF);

        Ok(self.buffer.get_u32())
    }
}

impl<B: BufMutExt> ProtocolWriter for BinaryProtocolSerializer<B> {
    type Final = B::Final;

    fn write_message_begin(&mut self, name: &str, type_id: MessageType, seqid: u32) {
        let version = (BINARY_VERSION_1 as u32) | (type_id as u32);
        self.write_i32(version as i32);
        self.write_string(name);
        self.write_u32(seqid);
    }

    #[inline]
    fn write_message_end(&mut self) {}

    #[inline]
    fn write_struct_begin(&mut self, _name: &str) {}

    #[inline]
    fn write_struct_end(&mut self) {}

    fn write_field_begin(&mut self, _name: &str, type_id: TType, id: i16) {
        self.write_byte(type_id as i8);
        self.write_i16(id);
    }

    #[inline]
    fn write_field_end(&mut self) {}

    #[inline]
    fn write_field_stop(&mut self) {
        self.write_byte(TType::Stop as i8)
    }

    fn write_map_begin(&mut self, key_type: TType, value_type: TType, size: usize) {
        self.write_byte(key_type as i8);
        self.write_byte(value_type as i8);
        self.write_i32(i32::try_from(size as u64).expect("map size overflow"));
    }

    #[inline]
    fn write_map_end(&mut self) {}

    fn write_list_begin(&mut self, elem_type: TType, size: usize) {
        self.write_byte(elem_type as i8);
        self.write_i32(i32::try_from(size as u64).expect("list size overflow"));
    }

    #[inline]
    fn write_list_end(&mut self) {}

    fn write_set_begin(&mut self, elem_type: TType, size: usize) {
        self.write_byte(elem_type as i8);
        self.write_i32(i32::try_from(size as u64).expect("set size overflow"));
    }

    fn write_set_end(&mut self) {}

    fn write_bool(&mut self, value: bool) {
        if value {
            self.write_byte(1)
        } else {
            self.write_byte(0)
        }
    }

    fn write_byte(&mut self, value: i8) {
        self.buffer.put_i8(value)
    }

    fn write_i16(&mut self, value: i16) {
        self.buffer.put_i16(value)
    }

    fn write_i32(&mut self, value: i32) {
        self.buffer.put_i32(value)
    }

    fn write_i64(&mut self, value: i64) {
        self.buffer.put_i64(value)
    }

    fn write_double(&mut self, value: f64) {
        self.buffer.put_f64(value)
    }

    fn write_float(&mut self, value: f32) {
        self.buffer.put_f32(value)
    }

    fn write_string(&mut self, value: &str) {
        self.write_i32(value.len() as i32);
        self.buffer.put_slice(value.as_bytes())
    }

    fn write_binary(&mut self, value: &[u8]) {
        self.write_i32(value.len() as i32);
        self.buffer.put_slice(value)
    }

    fn finish(self) -> B::Final {
        self.buffer.finalize()
    }
}

impl<B: Buf> ProtocolReader for BinaryProtocolDeserializer<B> {
    fn read_message_begin<F, T>(&mut self, msgfn: F) -> Result<(T, MessageType, u32)>
    where
        F: FnOnce(&[u8]) -> T,
    {
        let versionty = self.read_i32()? as u32;

        let msgtype = MessageType::try_from(versionty & !BINARY_VERSION_MASK)?; // !u32 -> ~u32
        let version = versionty & BINARY_VERSION_MASK;
        ensure_err!(version == BINARY_VERSION_1, ProtocolError::BadVersion);

        let name = {
            let len = self.read_i32()? as usize;
            let (len, name) = {
                if self.peek_bytes(len).is_some() {
                    let namebuf = self.peek_bytes(len).unwrap();
                    (namebuf.len(), msgfn(namebuf))
                } else {
                    let mut namebuf = Vec::new();
                    self.read_bytes_into(len, &mut namebuf)?;
                    (0, msgfn(namebuf.as_slice()))
                }
            };
            self.buffer.advance(len);
            name
        };
        let seq_id = self.read_u32()?;

        Ok((name, msgtype, seq_id))
    }

    fn read_message_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_struct_begin<F, T>(&mut self, namefn: F) -> Result<T>
    where
        F: FnOnce(&[u8]) -> T,
    {
        Ok(namefn(&[]))
    }

    fn read_struct_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_field_begin<F, T>(&mut self, fieldfn: F) -> Result<(T, TType, i16)>
    where
        F: FnOnce(&[u8]) -> T,
    {
        let type_id = TType::try_from(self.read_byte()?)?;
        let seq_id = match type_id {
            TType::Stop => 0,
            _ => self.read_i16()?,
        };
        Ok((fieldfn(&[]), type_id, seq_id))
    }

    fn read_field_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_map_begin(&mut self) -> Result<(TType, TType, usize)> {
        let k_type = TType::try_from(self.read_byte()?)?;
        let v_type = TType::try_from(self.read_byte()?)?;

        let size = self.read_i32()?;
        ensure_err!(size >= 0, ProtocolError::InvalidDataLength);
        Ok((k_type, v_type, size as usize))
    }

    fn read_map_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_list_begin(&mut self) -> Result<(TType, usize)> {
        let elem_type = TType::try_from(self.read_byte()?)?;
        let size = self.read_i32()?;
        ensure_err!(size >= 0, ProtocolError::InvalidDataLength);
        Ok((elem_type, size as usize))
    }

    fn read_list_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_set_begin(&mut self) -> Result<(TType, usize)> {
        let elem_type = TType::try_from(self.read_byte()?)?;
        let size = self.read_i32()?;
        ensure_err!(size >= 0, ProtocolError::InvalidDataLength);
        Ok((elem_type, size as usize))
    }

    fn read_set_end(&mut self) -> Result<()> {
        Ok(())
    }

    fn read_bool(&mut self) -> Result<bool> {
        match self.read_byte()? {
            0 => Ok(false),
            _ => Ok(true),
        }
    }

    fn read_byte(&mut self) -> Result<i8> {
        ensure_err!(self.buffer.remaining() >= 1, ProtocolError::EOF);

        Ok(self.buffer.get_i8())
    }

    fn read_i16(&mut self) -> Result<i16> {
        ensure_err!(self.buffer.remaining() >= 2, ProtocolError::EOF);

        Ok(self.buffer.get_i16())
    }

    fn read_i32(&mut self) -> Result<i32> {
        ensure_err!(self.buffer.remaining() >= 4, ProtocolError::EOF);

        Ok(self.buffer.get_i32())
    }

    fn read_i64(&mut self) -> Result<i64> {
        ensure_err!(self.buffer.remaining() >= 8, ProtocolError::EOF);

        Ok(self.buffer.get_i64())
    }

    fn read_double(&mut self) -> Result<f64> {
        ensure_err!(self.buffer.remaining() >= 8, ProtocolError::EOF);

        Ok(self.buffer.get_f64())
    }

    fn read_float(&mut self) -> Result<f32> {
        ensure_err!(self.buffer.remaining() >= 4, ProtocolError::EOF);

        Ok(self.buffer.get_f32())
    }

    fn read_string(&mut self) -> Result<String> {
        let vec = self.read_binary()?;

        Ok(String::from_utf8(vec)?)
    }

    fn read_binary(&mut self) -> Result<Vec<u8>> {
        let received_len = self.read_i32()?;
        ensure_err!(received_len >= 0, ProtocolError::InvalidDataLength);

        let received_len = received_len as usize;

        ensure_err!(self.buffer.remaining() >= received_len, ProtocolError::EOF);

        let mut bytes = Vec::with_capacity(received_len);
        self.read_bytes_into(received_len, &mut bytes)?;

        Ok(bytes)
    }
}

/// Serialize a Thrift value using the compact protocol.
pub fn serialize<T>(v: T) -> Bytes
where
    T: Serialize<BinaryProtocolSerializer<SizeCounter>>
        + Serialize<BinaryProtocolSerializer<BytesMut>>
        + Copy,
{
    let mut sizer = BinaryProtocolSerializer::with_buffer(SizeCounter::new());
    v.write(&mut sizer);

    let sz = sizer.finish();

    // Now that we have the size, allocate an output buffer and serialize into it
    let mut buf = BinaryProtocolSerializer::with_buffer(BytesMut::with_capacity(sz));
    v.write(&mut buf);

    // Done
    buf.finish()
}

/// Deserialize a Thrift blob using the compact protocol.
pub fn deserialize<T, B>(b: B) -> Result<T>
where
    B: AsRef<[u8]>,
    for<'a> T: Deserialize<BinaryProtocolDeserializer<Cursor<&'a [u8]>>>,
{
    let b = b.as_ref();
    let mut deser = BinaryProtocolDeserializer::new(Cursor::new(b));
    Ok(T::read(&mut deser)?)
}
