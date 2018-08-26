
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

        /// <returns>
        /// Return address of first free space to store a Node object
        /// or 0, if no space left.
        /// </returns>
        private static ushort GetFreeNodeAddr()
        {
            Debug.Assert(_maxNodeCount > 0);
            Debug.Assert(_firstNodeAddr == Mem.AddrFirst);

            for (ushort i = 0; i < _maxNodeCount; ++i)
            {
                ushort addr = (ushort)(Mem.AddrFirst + i * Node.NodeLen),
                    firstWord = Mem.LoadWord(addr);

                if (firstWord == Node.FreeFlagWord)
                {
                    return addr; // Found free space for a node.
                }
            }
            return 0; // No space for a node found.
        }

        /// <summary>
        /// Occupy heap space with one Node object that reserves the whole rest
        /// of heap space as one single unallocated node.
        /// </summary>
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
    }
}
