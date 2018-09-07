
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
            ushort nodeAddr = NodeMem.GetAllocNodeAddr(wantedLen),
                newNodeAddr = 0;
            Node n = null,
                newNode = null;

            if(nodeAddr == 0)
            {
                return 0;
            }

            n = Node.Load(nodeAddr);

            if(n.BlockLen == wantedLen)
            {
                n.IsAllocated = 1;
                return n.BlockAddr;
            }

            newNode = new Node();
            newNode.BlockLen = wantedLen;
            newNode.IsAllocated = 1;
            newNode.LastNodeAddr = nodeAddr;
            newNode.NextNodeAddr = n.NextNodeAddr;
            newNode.BlockAddr = (ushort)(n.BlockAddr + n.BlockLen - wantedLen);

            newNodeAddr = NodeMem.Store(newNode);
            if(newNodeAddr == 0)
            {
                return 0;
            }

            n.NextNodeAddr = newNodeAddr;

            if (newNode.NextNodeAddr != 0)
            {
                Node nextNode = Node.Load(n.NextNodeAddr);

                nextNode.LastNodeAddr = newNodeAddr;

                Node.Store(newNode.NextNodeAddr, nextNode);
            }

            return newNode.BlockAddr;
        }

        static void Main(string[] args)
        {
            NodeMem.Init();
            Alloc(400);
            //Alloc(500);
        }
    }
}
