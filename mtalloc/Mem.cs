
namespace mtalloc
{
    public static class Mem
    {
        private const ushort _memLen = 4096;

        public const ushort AddrFirst = 3094;
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
    }
}
