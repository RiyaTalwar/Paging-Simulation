# Paging-Simulation

Paging is a memory management scheme that eliminates the need for contiguous allocation of physical memory. This scheme permits the physical address space of a process to be non – contiguous. We have attempted to simulate paging of operating system using three different page replacement algorithms, them being - Least Recently Used, First in First Out and Least Frequently Used. To give an insight to this page replacement process and page mapping that happens between logical memory space and physical memory space, we depict a Page Map Table to the user whenever a page fault takes place. Along with this, the empty frames present in the physical memory space are also shown.

To run the above codes, open the terminal and run the following command
#g++ -o main vm.cpp 
#./main
