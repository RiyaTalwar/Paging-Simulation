#ifndef _PAGE_TABLE_
#define _PAGE_TABLE_

#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

class PageTable
{
 long long int ts;               // Counter to increment the timeStamp
 long long int pgFaults;         // To store the number of page faults that occur
 long long int pgReplace(int choice);      // Swap pages when there are no more available frames

 class PageTableEntry
 {
   public:
     long long int timeStamp;    //to keep track of pages refrenced recently
     long long int frameNo;      //represents a frame of physical memory
     int checkFrame;                  /*will have value 1 or 0 depending on whether
                                 the page is present in physical memory or not*/
     PageTableEntry()
     {
       timeStamp=0;
       frameNo=-1;
       checkFrame=0;
     }
 };

 vector<PageTableEntry> pgMap;   /*Represents all the Page table entries
                                 and is used to map logical address to physical address*/
 vector<long long int> emptyFrames;   //represents the empty frames in physical address space

 public:
   PageTable(long long int log_memSize,long long int phy_memSize);
   bool containsRef(long long int pgNum);
   void accessPg(long long int pgNum,int choice);
   void addPg(long long int pgNum,int choice);
   void displayPageTable();
   long long int getPageFaults();
   void setPageFaults(int pf);
   vector<long long int> fifo_framePgs;
   vector<pair<long long int,long long int> > lfu_framePgs;   //pair for keeping page and its corresponding frequency of reference


};

/*Constructor to create the page table with number of pages equal to logical memory size
and allocate empty frames in physical memory*/
PageTable::PageTable(long long int log_memSize,long long int phy_memSize)
{
 emptyFrames.clear();
 pgMap.clear();
 fifo_framePgs.clear();
 for(long long int i=phy_memSize-1;i>=0;i--)
 {
    emptyFrames.push_back(i);
 }
 pgMap.resize(log_memSize);
 ts=0;                             //intially timestamp must be 0
}

//To check whether a page is present in physical memory
bool PageTable::containsRef(long long int pgNum)
{
 return pgMap[pgNum].checkFrame;
}

//To access the frame corresponding to page entered by user
void PageTable::accessPg(long long int pgNum,int choice)
{
 long long int frame=pgMap[pgNum].frameNo;

 if(choice==3)       //this loop is exclusively for lfu algorithm
 {
   for(long long int i=0;i<lfu_framePgs.size();i++)      //since the page is already present in physical memory, increment the frequency
   {
     if(lfu_framePgs[i].first==pgNum)
     {
       long long int freq=lfu_framePgs[i].second;
       lfu_framePgs[i].second=freq+1;
     }
   }
 }

 cout<<"The page "<<pgNum<<" was found at frame "<<frame<<" in physical memory\n\n";
 pgMap[pgNum].timeStamp=ts;
 ++ts;
}

//To add this page in physical memory
void PageTable::addPg(long long int pgNum,int choice)
{

 //if there are empty frames in physical memory, then add the page in the empty frames
 if(!emptyFrames.empty())
 {
   long long int frame=emptyFrames.back();
   emptyFrames.pop_back();
   pgMap[pgNum].frameNo=frame;
   pgMap[pgNum].checkFrame=1;
   pgMap[pgNum].timeStamp=ts;
   cout<<"time "<<ts<<"\n";
   cout<<"Page Fault: Adding "<<pgNum<<" at frame "<<frame<<"\n";
 }
 /*if there are no empty frames in physical memory, then swap a page present in Physical
 Memory with this page using the page replacement algorithm the user wants*/
 else
 {
   long long int frame=pgReplace(choice);
   pgMap[pgNum].frameNo=frame;
   pgMap[pgNum].checkFrame=1;
   pgMap[pgNum].timeStamp=ts;
   cout<<"time "<<ts<<"\n";
   cout<<"Page Fault: Adding "<<pgNum<<" at frame "<<frame<<"\n";
 }
 ++pgFaults;
 fifo_framePgs.push_back(pgNum);
 lfu_framePgs.push_back(make_pair(pgNum,1));
 displayPageTable();
}

/*use different page replacement algorithm depending on the choice entered by
the user and find a page that should be replaced*/
long long int PageTable::pgReplace(int choice)
{

 if(choice==1)         //Least Recently Used
 {
   cout<<"time "<<ts<<"\n";
   long long int min=ts;
   long long int replacePg;
   for(long long int j=0;j<pgMap.size();j++)
   {
     if(pgMap[j].checkFrame==1)     //if the page is present in physical memory
     {
       if(pgMap[j].timeStamp<min)    //if this page is less recent compared to the previous page in physical memory frame
       {
         min=pgMap[j].timeStamp;   //update the minimum timestamp
         replacePg=j;              //the page to be replaced is with this minimum timestamp now
       }
     }
   }
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].checkFrame=0;
   pgMap[replacePg].frameNo=-1;
   return frame;                   //return the frame where the new page will be inserted
 }

 else if(choice==2)    //First in First Out
 {
   long long int replacePg=fifo_framePgs.at(0);         //removing the first page that
   fifo_framePgs.erase(fifo_framePgs.begin()+0);             //was inserted in the physical memory
   pgMap[replacePg].checkFrame=0;
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].frameNo=-1;
   return frame;                  //return the frame where the new page will be inserted
 }


 else if(choice==3)    //Least frequently used
 {
   long long int min=0;
   for(long long int i=0;i<lfu_framePgs.size();i++)    //loop to find the page in physical memory with minimum frequency
   {
     if(lfu_framePgs[i].second<lfu_framePgs[min].second)
       min=i;
   }
   long long int replacePg=lfu_framePgs[min].first;
   lfu_framePgs.erase(lfu_framePgs.begin()+min);
   pgMap[replacePg].checkFrame=0;
   long long int frame=pgMap[replacePg].frameNo;
   pgMap[replacePg].frameNo=-1;
   return frame;                  //return the frame where the new page will be inserted
 }
 return 1;
}

//display the contents of the page table
void PageTable::displayPageTable()
{
 // printing top border
 cout << left << setfill('-') << setw(1) << "+" << setw(4) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << setfill('-') << "-" << right << "+" << setfill(' ')<< setw(21) << "+" << setfill('-') << setw(15) << "-" << endl;
 // printing table record
 cout << right << setw(1) << "|" << "ADR" << setfill(' ') << setw(2) << "|" << left << setw(15) << "Frame no." << setw(1) << "|" << setw(15)  << "Valid" << setw(1) << "|" << setw(5) << "TimeStamp" << setw(1) << "|" << setw(20) << setfill(' ') << " " << "|" << "Empty Frames" << setw(4) << right << "|" << endl;
 // printing bottom border
 cout << left << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << setw(20) << setfill(' ') << " " << setw(1) << setfill(' ') << "+" << setfill('-') << setw(15) << "-" << endl;
 for (unsigned int i = 0; i < pgMap.size(); i++)
 {
   // printing table record
   cout << "|" << setfill(' ') << setw(3) << i << setfill(' ') << setw(1) << " " << "|" << setw(15) << pgMap.at(i).frameNo
     << setw(1) << "|" << setw(15) << pgMap.at(i).checkFrame << setw(1) << "|" << setw(5)
     << pgMap.at(i).timeStamp << setfill(' ') << setw(4) << " " << setw(1) << "|";
   // conditionally print the empty frames
   if (i < emptyFrames.size()) {
     cout << left << setw(20) << setfill(' ') << " " << "|" << setw(5) << emptyFrames.at(i) << setfill(' ') << setw(9) << " " << setw(1) << "|" <<endl;
     //printing bottom border
     cout << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << setw(20) << setfill(' ') << " " << setw(1) << setfill(' ') << "+" << setfill('-') << setw(15) << "-" << endl;
   }
   // if there wasn't a frame printed don't forget to print the bottom border
   else {
     // printing bottom border
     cout << endl << setfill('-') << setw(4) << "+" << setw(17) << "-" << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" << setw(9) << "-" << setw(1) << "+" << endl;
   }
 }

}

//initilize page faults
void PageTable::setPageFaults(int pf)
{
 pgFaults=pf;
}

//Returns the number of page faults
long long int PageTable::getPageFaults()
{
 return pgFaults;
}

#endif
