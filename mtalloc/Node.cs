
namespace mtalloc
{
    public class Node
    {
        public ushort LastNodeAddr { get; set; }
        public ushort BlockAddr { get; set; }
        public ushort BlockLen { get; set; }
        public byte IsAllocated { get; set; }
        public ushort NextNodeAddr { get; set; }

        public static Node Load(ushort nodeAddr)
        {
            var retVal = new Node();
            var addr = nodeAddr;

            retVal.LastNodeAddr = Mem.LoadWord(addr);
            addr += 2;

            retVal.BlockAddr = Mem.LoadWord(addr);
            addr += 2;

            retVal.BlockLen = Mem.LoadWord(addr);
            addr += 2;

            retVal.IsAllocated = Mem.LoadByte(addr);
            ++addr;

            retVal.NextNodeAddr = Mem.LoadWord(addr);

            return retVal;
        }

        public static void Store(ushort nodeAddr, Node node)
        {
            var addr = nodeAddr;

            // Pointer to last free block node:

            Mem.StoreWord(addr, node.LastNodeAddr);
            addr += 2;

            // Start address of free block:

            Mem.StoreWord(addr, node.BlockAddr);
            addr += 2;

            // Length of free block:

            Mem.StoreWord(addr, node.BlockLen);
            addr += 2;

            // Is node allocated or free?

            Mem.StoreByte(addr, node.IsAllocated);
            ++addr;

            // Pointer to next free block node:

            Mem.StoreWord(addr, node.NextNodeAddr);
        }
    }
}
