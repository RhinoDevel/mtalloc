
using System;

namespace mtalloc
{
    class Program
    {
        private static void Free(ushort blockAddr)
        {
            throw new NotImplementedException();
        }

        private static ushort Alloc(ushort wantedLen)
        {
            throw new NotImplementedException();
        }

        static void Main(string[] args)
        {
            NodeMem.Init();
            //Alloc(500);
            //Alloc(500);
        }
    }
}
