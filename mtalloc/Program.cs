using System;
using System.Collections.Generic;

namespace mtalloc
{
    class Program
    {
        private const ushort _memLen = 4096;
        private const ushort _addrFirst = 3094;
        private const ushort _heapLen = _memLen - _addrFirst - 2;

        private static readonly byte[] _mem = new byte[_memLen];

        private static void Fill(ushort val, out byte low, out byte high)
        {
            low = (byte)(val & 0xFF);
            high = (byte)(val >> 8);
        }

        private static void Store(ushort addr, ushort val)
        {
            Fill(val, out _mem[addr], out _mem[addr + 1]);
        }

        private static ushort Load(ushort addr)
        {
            return (ushort)(_mem[addr] | (_mem[addr + 1] << 8));
        }

        private static void LoadNode(
            ushort nodeAddr,
            out ushort lastNodeAddr,
            out ushort blockAddr,
            out ushort blockLen,
            out byte isAllocated,
            out ushort nextNodeAddr)
        {
            var addr = nodeAddr;

            lastNodeAddr = Load(addr);
            addr += 2;

            blockAddr = Load(addr);
            addr += 2;

            blockLen = Load(addr);
            addr += 2;

            isAllocated = _mem[addr];
            ++addr;

            nextNodeAddr = Load(addr);
        }

        private static void UpdateNodeLastNodeAddr(
            ushort nodeAddr, ushort lastNodeAddr)
        {
            Store(nodeAddr, lastNodeAddr);
        }

        private static void UpdateNodeBlockLen(
            ushort nodeAddr, ushort blockLen)
        {
            Store((ushort)(nodeAddr + 4), blockLen);
        }

        private static void UpdateNodeIsAllocated(
            ushort nodeAddr, byte isAllocated)
        {
            _mem[nodeAddr + 6] = isAllocated;
        }

        private static void UpdateNodeNextNodeAddr(
            ushort nodeAddr, ushort nextNodeAddr)
        {
            Store((ushort)(nodeAddr + 7), nextNodeAddr);
        }

        private static void StoreNode(
            ushort nodeAddr,
            ushort lastNodeAddr,
            ushort blockAddr,
            ushort blockLen,
            byte isAllocated,
            ushort nextNodeAddr)
        {
            var addr = nodeAddr;

            // Pointer to last free block node:

            Store(addr, lastNodeAddr);
            addr += 2;

            // Start address of free block:

            Store(addr, blockAddr);
            addr += 2;

            // Length of free block:

            Store(addr, blockLen);
            addr += 2;

            // Is node allocated or free?

            _mem[addr] = isAllocated;
            ++addr;

            // Pointer to next free block node:

            Store(addr, nextNodeAddr);
        }

        private static void Init()
        {
            StoreNode(_addrFirst, 0, _addrFirst, _heapLen, 0, 0);
        }

        private static void Free(ushort blockAddr)
        {
            throw new NotImplementedException();
        }

        private static ushort Alloc(ushort wantedLen)
        {
            ushort addr, candBlockLen, candBlockNodeAddr;

            if (wantedLen > _heapLen) // Just a shortcut..
            {
                return 0;
            }
            addr = _addrFirst;
            candBlockNodeAddr = 0;
            candBlockLen = 0xFFFF;//ushort.MaxValue;
            do
            {
                ushort lastNodeAddr, blockAddr, blockLen, nextNodeAddr;
                byte isAllocated;

                LoadNode(
                    addr,
                    out lastNodeAddr,
                    out blockAddr,
                    out blockLen,
                    out isAllocated,
                    out nextNodeAddr);

                if (isAllocated == 0)
                {
                    if (blockLen == wantedLen) // Would fit perfectly.
                    {
                        candBlockNodeAddr = addr;
                        candBlockLen = blockLen;
                        break;
                    }

                    if (blockLen > wantedLen) // Would fit.
                    {
                        if (candBlockLen > blockLen)
                        {
                            // Would fit better than current best candidate
                            // or no candidate, yet.
                            //
                            // Update best candidate:

                            candBlockNodeAddr = addr;
                            candBlockLen = blockLen;
                        }
                    }
                }

                addr = nextNodeAddr;
            } while (addr != 0);

            if (candBlockNodeAddr == 0)
            {
                return 0; // No free block with sufficient length found.
            }

            {
                ushort lastNodeAddr, blockAddr, blockLen, nextNodeAddr;
                byte isAllocated;

                LoadNode(
                    candBlockNodeAddr,
                    out lastNodeAddr,
                    out blockAddr,
                    out blockLen,
                    out isAllocated,
                    out nextNodeAddr);

                if (candBlockLen > wantedLen)
                {
                    // Update node entry (taking higher part of block):

                    short newBlockLen;

                    // Check, if there is space left for another node:
                    //
                    throw new NotImplementedException();

                    newBlockLen = (ushort)(blockLen - wantedLen);

                    UpdateNodeBlockLen(
                        candBlockNodeAddr, newBlockLen);

                    // Create and insert new node entry for allocated part:
                    //
                    throw new NotImplementedException();

                    return (ushort)(blockAddr + newBlockLen);
                }

                // Mark node as referencing an allocated block:

                UpdateNodeIsAllocated(candBlockNodeAddr, 1);

                return blockAddr;
            }
        }

        static void Main(string[] args)
        {
            l[1] = null;

            Init();
            Alloc(500);
            Alloc(500);
        }
    }
}
