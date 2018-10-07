
using System;
using System.Diagnostics;

namespace mtalloc
{
    public static class Mem
    {
        private const ushort _memLen = 80;

        public const ushort AddrFirst = 8;
        public const ushort HeapLen = _memLen - AddrFirst;

        private static readonly byte[] _mem = new byte[_memLen];

        private static void Fill(ushort val, out byte low, out byte high)
        {
            low = (byte)(val & 0xFF);
            high = (byte)(val >> 8);
        }

        public static void StoreWord(ushort addr, ushort val)
        {
            Fill(val, out _mem[addr], out _mem[addr + 1]);
        }

        public static ushort LoadWord(ushort addr)
        {
            return (ushort)(_mem[addr] | (_mem[addr + 1] << 8));
        }

        public static void StoreByte(ushort addr, byte val)
        {
            _mem[addr] = val;
        }

        public static byte LoadByte(ushort addr)
        {
            return _mem[addr];
        }

        public static void Clear(ushort addr, ushort len, byte val = 0)
        {
            var lim = addr + len;

            for(var cur = addr;cur < lim; ++cur)
            {
                StoreByte(cur, val);
            }
        }

        public static void Print()
        {
            const ushort columns = 16;

            Debug.Assert(columns < 256);

            {
                var rowStrings = new string[columns];

                for(ushort i = 0;i < columns;++i)
                {
                    rowStrings[i] = string.Format("{0:X2}", i);
                }
                Console.WriteLine(string.Join(" ", rowStrings));
            }

            for(ushort i = 0;i < _memLen; i += columns)
            {
                var rowAddr = i;
                var rowStrings = new string[columns];

                for(ushort j = 0;j < columns; ++j)
                {
                    var colAddr = rowAddr + j;

                    if(colAddr == _memLen)
                    {
                        break;
                    }
                    Debug.Assert(colAddr <  _memLen);

                    rowStrings[j] = string.Format("{0:X2}", _mem[colAddr]);
                }
                Console.WriteLine(string.Join(" ", rowStrings));
            }
        }
    }
}
