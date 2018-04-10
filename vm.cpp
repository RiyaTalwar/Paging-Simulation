#include<iostream>
#include "pagetable.h"
using namespace std;

bool checkNo(long long int);

int main()
{
  long long int log_memSize, phy_memSize,pgNum,n;
  int choice;

  //Size of logical memory space
  cout<<"Enter the number of pages in logical memory space \n";
  cin>>log_memSize;
  if(!checkNo(log_memSize))
  {
    cout<<"Error: the number of pages entered is invalid\n";
    exit(EXIT_FAILURE);
  }

  //Size of physical memory space
  cout<<"Enter the number of frames in physical memory space \n";
  cin>>phy_memSize;
  if(!checkNo(phy_memSize))
  {
    cout<<"Error: the number of frames entered is invalid\n";
    exit(EXIT_FAILURE);
  }

  do {

    //Create page table using the logical memory size and physical memory size obtained
    PageTable pgTable(log_memSize,phy_memSize);

    //Select the page replacement algoritm for paging simulation
    cout<<"Choose a page replacement algorithm for paging simulation (1 - 5):\n";
    cout<<"1. Least Recently Used\n";
    cout<<"2. First in First Out\n";
    cout<<"3. Least frequently used\n";
    cout<<"4. Exit\n";
    cin>>choice;
    if(choice==4)
      break;
    pgTable.setPageFaults(0);

    /*Pages that are there in the Logical Memory Space are entered by the
    User and the Page Table is used to get reference of the corresponding frames in
    Physical Memory Space*/
    cout<<"Enter the number of pages to be refered\n";
    cin>>n;
    long long int pgArr[n];
    cout<<"Enter all the page numbers to be refered each should belong to this range (0 - "<<log_memSize-1<<"):\n";
    for(long long int i=0;i<n;i++)
    {
          cin>>pgArr[i];
          pgNum=pgArr[i];
          if(pgNum<0 || pgNum>log_memSize-1)       //if the input is out of range
          {
              cout<<"Error! Enter the page number in range (0 - "<<log_memSize-1<<")\n";
              i=i-1;
          }
          else
          {
              if(pgTable.containsRef(pgNum))       //if the page is in physical memory
              {
                pgTable.accessPg(pgNum,choice);           //access the frame corresponding to that page
              }
              else                                 //if the page is not in physical memory
              {
                pgTable.addPg(pgNum,choice);       //add this page in physical memory
                pgTable.accessPg(pgNum,choice);           //access the frame corresponding to that page
              }
          }

      }    //the above loop will run till the end of input

  cout<<"The page faults occured are : "<<pgTable.getPageFaults()<<"\n";

}while(choice!=4);

  return 0;
}

//This function checks whether the number of pages or frames entered by the user is valid
bool checkNo(long long int size)
{
  if(size<0)
    return false;
    else
      return true;
}
