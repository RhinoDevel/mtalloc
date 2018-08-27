
using System;

namespace mtalloc
{
    class Program
    {
        //private static void UpdateNodeLastNodeAddr(
        //    ushort nodeAddr, ushort lastNodeAddr)
        //{
        //    Mem.StoreWord(nodeAddr, lastNodeAddr);
        //}

        //private static void UpdateNodeBlockLen(
        //    ushort nodeAddr, ushort blockLen)
        //{
        //    Mem.StoreWord((ushort)(nodeAddr + 4), blockLen);
        //}

        //private static void UpdateNodeIsAllocated(
        //    ushort nodeAddr, byte isAllocated)
        //{
        //    Mem.StoreByte(nodeAddr, isAllocated);
        //}

        //private static void UpdateNodeNextNodeAddr(
        //    ushort nodeAddr, ushort nextNodeAddr)
        //{
        //    Mem.StoreWord((ushort)(nodeAddr + 7), nextNodeAddr);
        //}

        private static void Free(ushort blockAddr)
        {
            throw new NotImplementedException();
        }

        private static ushort Alloc(ushort wantedLen)
        {
            ushort addr = NodeMem.FirstNodeAddr,
                firstBlockAddr = NodeMem.GetFirstBlockAddr();
            Node cand = null;

            do
            {
                var n = Node.Load(addr);

                if (n.IsAllocated == 0)
                {
                    if (n.BlockAddr != firstBlockAddr // Not first block.
                        && n.BlockLen == wantedLen) // Would fit perfectly.
                    {
                        cand = n;
                        break;
                    }
                    else
                    {
                        if (n.BlockLen > wantedLen) // Would fit.
                        {
                            if (n.BlockAddr != firstBlockAddr)
                            {
                                if (cand == null || cand.BlockLen > n.BlockLen)
                                {
                                    // Would fit better than current
                                    // best candidate or no candidate, yet.
                                    //
                                    // Update best candidate:

                                    cand = n;
                                }
                            }
                            else
                            {
                                if(cand == null)
                                {
                                    cand = n;
                                }
                            }
                        }
                    }
                }

                addr = n.NextNodeAddr;
            } while (addr != 0);

            if (cand == null)
            {
                return 0; // No free block with sufficient length found.
            }

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
