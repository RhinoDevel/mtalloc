
using System;
using System.Diagnostics;

namespace mtalloc
{
    class Program
    {
        private static void Free(ushort blockAddr)
        {
            ushort nodeAddr = 0;
            Node cur = null;

            if(blockAddr == 0)
            {
                //Debug.Assert(false);
                return; // Nothing to do.
            }

            nodeAddr = NodeMem.GetBlockNodeAddr(blockAddr);
            if(nodeAddr == 0)
            {
                Debug.Assert(false);
                return; // No block at given (start) address is allocated.
            }

            cur = Node.Load(nodeAddr);

            if(cur.IsAllocated == 0)
            {
                Debug.Assert(false);
                return; // Already deallocated.
            }

            cur.IsAllocated = 0;
            Node.Store(nodeAddr, cur);

            if(cur.NextNodeAddr != 0)
            {
                NodeMem.MergeUnallocatedWithNextIfPossible(nodeAddr);
            }
            if(cur.LastNodeAddr != 0
                && Node.Load(cur.LastNodeAddr).IsAllocated == 0)
            {
                NodeMem.MergeUnallocatedWithNextIfPossible(cur.LastNodeAddr);
            }
        }

        private static ushort Alloc(ushort wantedLen)
        {
            ushort nodeAddr = NodeMem.GetAllocNodeAddr(wantedLen),
                newNodeAddr = 0;
            Node n = null,
                newNode = null;

            if(nodeAddr == 0)
            {
                return 0;
            }

            n = Node.Load(nodeAddr);

            Debug.Assert(n.IsAllocated == 0);

            if(n.BlockLen == wantedLen)
            {
                n.IsAllocated = 1;
                Node.Store(nodeAddr, n);
                return n.BlockAddr;
            }

            newNode = new Node();
            newNode.BlockLen = wantedLen;
            newNode.IsAllocated = 1;
            newNode.LastNodeAddr = nodeAddr;
            newNode.NextNodeAddr = n.NextNodeAddr;
            newNode.BlockAddr = (ushort)(n.BlockAddr + n.BlockLen - wantedLen);

            newNodeAddr = NodeMem.Store(newNode);
            n = null;
            if(newNodeAddr == 0)
            {
                return 0;
            }
            n = Node.Load(nodeAddr);
            
            n.BlockLen -= newNode.BlockLen;
            n.NextNodeAddr = newNodeAddr;
            Node.Store(nodeAddr, n);

            if (newNode.NextNodeAddr != 0)
            {
                Node nextNode = Node.Load(newNode.NextNodeAddr);

                nextNode.LastNodeAddr = newNodeAddr;

                Node.Store(newNode.NextNodeAddr, nextNode);
            }

            return newNode.BlockAddr;
        }

        static void Main(string[] args)
        {
            NodeMem.Init();
            
            var a = Alloc(10);
            var b = Alloc(20);

            
            var c = Alloc(30);
            Free(b);
            Free(c);
            Free(a);
        }
    }
}
