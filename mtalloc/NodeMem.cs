
using System;
using System.Diagnostics;

namespace mtalloc
{
    public static class NodeMem
    {
        private static ushort _maxNodeCount = 0;
        //
        // How many nodes can currently be stored.

        private static ushort _firstNodeAddr = 0; // First node's address.

        public static void Init()
        {
            ushort firstBlockAddr, firstBlockLen;
            Node firstNode;

            Debug.Assert(_maxNodeCount == 0);
            Debug.Assert(_firstNodeAddr == 0);

            _maxNodeCount = 1;
            _firstNodeAddr = Mem.AddrFirst;

            // The first (and initially only) block will occupy the complete
            // last part of heap space after the first node's memory space:
            //
            firstBlockAddr = (ushort)(_firstNodeAddr + Node.NodeLen);
            firstBlockLen = Mem.HeapLen - Node.NodeLen;

            firstNode = new Node
            {
                LastNodeAddr = 0,
                BlockAddr = firstBlockAddr,
                BlockLen = firstBlockLen,
                IsAllocated = 0,
                NextNodeAddr = 0
            };

            Node.Store(_firstNodeAddr, firstNode);
        }

        public static ushort GetFreeNodeAddr()
        {
            for (ushort i = 0; i < _maxNodeCount; ++i)
            {
                ushort addr = (ushort)(Mem.AddrFirst + i * Node.NodeLen),
                    firstWord = Mem.LoadWord(addr);

                if (firstWord == Node.FreeFlagWord)
                {
                    return addr; // Found free space for a node.
                }
            }

            // No free space for a node found, try to increase node space:

            if(_firstNodeAddr == 0)
            {
                Init(); // Initialize node space and first node.

                return _firstNodeAddr;
            }

            throw new NotImplementedException(); // MT_TODO: TEST: Implement!
        }
    }
}
