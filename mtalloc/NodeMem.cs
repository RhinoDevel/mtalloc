
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

        private static ushort GetFirstBlockAddr()
        {
            Debug.Assert(_firstNodeAddr == Mem.AddrFirst);
            Debug.Assert(_maxNodeCount > 0);

            // First block begins behind last node.

            return (ushort)(_firstNodeAddr + Node.NodeLen * _maxNodeCount);
        }

        /// <summary>
        /// Mark node's space in memory at given position as free for reuse.
        /// </summary>
        private static void MarkNodeSpaceAsFree(ushort nodeAddr)
        {
            Debug.Assert(_firstNodeAddr == Mem.AddrFirst);
            Debug.Assert(nodeAddr >= _firstNodeAddr);
            Debug.Assert(nodeAddr <= GetFirstBlockAddr());
            Debug.Assert((nodeAddr - _firstNodeAddr) % Node.NodeLen == 0);

            Mem.StoreWord(nodeAddr, Node.FreeFlagWord);
        }

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
                ushort addr = (ushort)(_firstNodeAddr + i * Node.NodeLen),
                    firstWord = Mem.LoadWord(addr);

                if (firstWord == Node.FreeFlagWord)
                {
                    return addr; // Found free space for a node.
                }
            }
            return 0; // No space for a node found.
        }

        public static ushort GetBlockNodeAddr(ushort blockAddr)
        {
            Debug.Assert(blockAddr > 0);
            Debug.Assert(_maxNodeCount > 0);
            Debug.Assert(_firstNodeAddr == Mem.AddrFirst);

            ushort retVal = 0;

            for (ushort i = 0; i < _maxNodeCount; ++i)
            {
                ushort addr = (ushort)(_firstNodeAddr + i * Node.NodeLen),
                    firstWord = Mem.LoadWord(addr);
                Node node;

                if (firstWord == Node.FreeFlagWord)
                {
                    continue; // Free space for a node.
                }

                node = Node.Load(addr);
                if(node.BlockAddr == blockAddr)
                {
                    retVal = addr;
                    break;
                }
            }
            Debug.Assert(retVal > 0 && retVal < 0xFFFF);//ushort.MaxValue);
            return retVal;
        }

        private static ushort GetFirstBlockNodeAddr()
        {
            return GetBlockNodeAddr(GetFirstBlockAddr());
        }

        private static ushort CreateFreeNodeAddr()
        {
            Debug.Assert(GetFreeNodeAddr() == 0);

            var firstBlockNodeAddr = GetFirstBlockNodeAddr();
            var firstBlockNode = Node.Load(firstBlockNodeAddr);
            ushort freeNodeAddr;

            // Is first block's node unallocated?
            //
            if(firstBlockNode.IsAllocated != 0)
            {
                return 0;
            }

            // Is first block's length equal or longer than a node's length
            // plus one (at least one byte must be on stack for new node to
            // be created for this to makes sense..)?
            //
            if(firstBlockNode.BlockLen < Node.NodeLen + 1)
            {
                return 0;
            }

            freeNodeAddr = firstBlockNode.BlockAddr;
            firstBlockNode.BlockAddr += Node.NodeLen;
            firstBlockNode.BlockLen -= Node.NodeLen;
            ++_maxNodeCount;
            MarkNodeSpaceAsFree(freeNodeAddr);

            Debug.Assert(GetFreeNodeAddr() == freeNodeAddr);
            return freeNodeAddr;
        }

        /// <returns>Stored node's address or 0, if no space left.</returns>
        public static ushort Store(Node node)
        {
            Debug.Assert(node != null);

            var nodeAddr = GetFreeNodeAddr();

            if(nodeAddr == 0)
            {
                nodeAddr = CreateFreeNodeAddr();
                if(nodeAddr == 0)
                {
                    return 0;
                }
            }
            Node.Store(nodeAddr, node);
            return nodeAddr;
        }

        /// <returns>
        /// Address of node with unallocated block of sufficient size to hold
        /// given/wanted amount of bytes.
        /// 
        /// 0, if no such node was found.
        /// </returns>
        public static ushort GetAllocNodeAddr(ushort wantedLen)
        {
            Debug.Assert(wantedLen > 0);

            ushort retVal = 0,
                firstBlockAddr = NodeMem.GetFirstBlockAddr(),
                blockLen = 0;
            Node n = null;

            for (ushort addr = NodeMem._firstNodeAddr;
                addr != 0;
                addr = n.NextNodeAddr)
            {
                n = Node.Load(addr);

                if (n.IsAllocated != 0)
                {
                    continue; // Node's block is already allocated.
                }

                if (n.BlockLen < wantedLen)
                {
                    continue; // Would not fit in node's block.
                }

                if (n.BlockAddr == firstBlockAddr)
                {
                    if (retVal == 0)
                    {
                        retVal = addr; // No better candidate found, yet.
                        blockLen = n.BlockLen;
                    }
                    continue;
                }

                if (n.BlockLen == wantedLen)
                {
                    retVal = addr;
                    break; // Fits perfectly. Found!
                }

                if (retVal == 0)
                {
                    retVal = addr;
                    blockLen = n.BlockLen;
                    continue; // No better candidate found, yet.
                }

                if (blockLen > n.BlockLen)
                {
                    // Would fit better than current best candidate.

                    retVal = addr;
                    blockLen = n.BlockLen;
                }
            }
            return retVal;
        }

        /// <summary>
        /// Occupy heap space with one Node object that reserves the whole rest
        /// of heap space as one single unallocated node.
        /// </summary>
        public static void Init()
        {
            Debug.Assert(_maxNodeCount == 0);
            Debug.Assert(_firstNodeAddr == 0);

            ushort firstBlockAddr, firstBlockLen;
            Node firstNode;

            _maxNodeCount = 1;
            _firstNodeAddr = Mem.AddrFirst;

            // The first (and initially only) block will occupy the complete
            // last part of heap space after the first node's memory space:
            //
            firstBlockAddr = GetFirstBlockAddr();
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
