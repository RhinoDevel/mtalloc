﻿
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

#if DEBUG
            Mem.Clear(cur.BlockAddr, cur.BlockLen, 0xCD);
#endif //DEBUG

            if(cur.NextNodeAddr != 0)
            {
                NodeMem.MergeUnallocatedWithNextIfPossible(nodeAddr);
            }
            if(cur.LastNodeAddr != 0
                && Node.Load(cur.LastNodeAddr).IsAllocated == 0)
            {
                NodeMem.MergeUnallocatedWithNextIfPossible(cur.LastNodeAddr);
            }

            NodeMem.LimitFreeNodes();
        }

        private static ushort Alloc(ushort wantedLen)
        {
            ushort newNodeAddr = 0,
                nodeAddr = 0;
            Node n = null,
                newNode = null;
            
            if(wantedLen == 0)
            {
                return 0;
            }

            // Make sure that there is a node space available, first
            // (and maybe frees space from first node, which must
            // be done BEFORE first node may gets selected as
            // "alloc" node):
            //
            if(!NodeMem.TryToReserveNodeSpace())
            {
                return 0; // No more space for another node available.
            }

            nodeAddr = NodeMem.GetAllocNodeAddr(wantedLen);
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
            
            Debug.Assert(n.BlockLen > newNode.BlockLen);

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

            var all = Alloc(Mem.HeapLen - 2 * Node.NodeLen);

            Mem.Clear(all, Mem.HeapLen - 2 * Node.NodeLen, 0xBC);

            Mem.Print();
            Console.WriteLine("***");

            Free(all);
            all = 0;

            var a = Alloc(10);

            Mem.Clear(a, 10, 170);
            
            var b = Alloc(5);

            Mem.Clear(b, 5, 187);
            
            var c = Alloc(7);

            Mem.Clear(c, 7, 204);
            
            Mem.Print();
            Console.WriteLine("-");

            Free(c);

            Mem.Print();
            Console.WriteLine("-");

            Free(b);

            Mem.Print();
            Console.WriteLine("-");

            Free(a);

            Mem.Print();
        }
    }
}
