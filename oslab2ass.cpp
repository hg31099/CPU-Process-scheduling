#include <bits/stdc++.h>
using namespace std;

typedef struct process_tag
{
    int art;//arrival time
    int bt;
    int rt;
    int iobt;
    int ioqt;
    int iort;
    int priority;
    int state;/*0 for Not arrived
                1 for Ready
                2 for Running
                3 for I/O
                4 for wait for I/o
                5 for Completed
                */
    int rest;
    int tt;
    int wt;
}process;

int check(process p[],int n)//Returns 0 if all are not finished else returns 1
{
    int ret=1;
    for(int i=0;i<n && ret==1;i++)
    {
        if(p[i].state!=5)
        {
            ret=0;
        }
    }
    return ret;
}
void intialise1(process p[],int n)
{
    p[0].art=0;
    p[0].bt=p[0].rt=18;
    p[0].iobt=5;
    p[0].ioqt=5;
    p[0].priority=0;
    p[0].state=0;

    p[1].art=3;
    p[1].bt=p[1].rt=27;
    p[1].iobt=6;
    p[1].ioqt=6;
    p[1].priority=0;
    p[1].state=0;

    p[2].art=8;
    p[2].bt=p[2].rt=44;
    p[2].iobt=4;
    p[2].ioqt=6;
    p[2].priority=0;
    p[2].state=0;

    p[3].art=12;
    p[3].bt=p[3].rt=50;
    p[3].iobt=3;
    p[3].ioqt=9;
    p[3].priority=0;
    p[3].state=0;
}
 /*void intialise2(process p[],int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<"\n For Process  "<<i<<"  : \n";
        cout<<" Arrival Time    : ";
        cin>>p[i].art;
        cout<<" CPU Burst Time  : ";
        cin>>p[i].bt;
        p[i].rt=p[i].bt;
        cout<<" I/O Burst Time  : ";
        cin>>p[i].iobt;
        cout<<" I/O Burst after : ";
        cin>>p[i].ioqt;
        p[i].priority=p[i].state=0;
    }
}
*/

int main()
{
    int a,b,n;
    int cs=0;//count of context switches
    queue <int> ready;
    queue <int> io;
    //cout<<" Enter number of processes : ";
    //cin>>n;
    n=4;
    process p[n];
    intialise1(p,n);
    //intialise2(p,n);
    cout<<"\n Enter value of Alpha : ";
    cin>>a;
    cout<<"\n Enter value of Beta  : ";
    cin>>b;
    int curr_time=0,curr_process=-1,curr_io=-1;
    while(check(p,n)==0)
    {
	//Check for Arrival
        for(int i=0;i<n;i++)
        {
            if(p[i].state==0 && p[i].art==curr_time)//arrival of new process
            {
                p[i].state=1;//in ready state
                p[i].priority=0;
                printf("\n -> Process %d arrived at time %d.\n",i,curr_time);
                ready.push(i);
            }
        }
        if(curr_process!=-1)// Some process is running
        {
            if(p[curr_process].rt==0)// Process completion
            {
                if(p[curr_process].bt%p[curr_process].ioqt==0)// I/O after process completion
                {
                    p[curr_process].state=4;// wait for I/O execution
                    p[curr_process].iort=p[curr_process].iobt;
                    cout<<"\n CPU burst of process "<<curr_process<<" ended at time "<<curr_time<<".\n";
                    io.push(curr_process);//pushed in I/O queue
                }
                else
                {
                    p[curr_process].state=5;//Completed
                    cout<<"\n CPU burst of process "<<curr_process<<" ended at time "<<curr_time<<".\n";
                    printf("\n -> Process %d Completed at time %d.\n",curr_process,curr_time);
                    p[curr_process].tt = curr_time - p[curr_process].art;
                }
                curr_process=-1;
            }
            else if( (p[curr_process].bt-p[curr_process].rt)%p[curr_process].ioqt == 0 )//Checks for I/O if process is not completed
            {
                p[curr_process].state=4;// wait for I/O execution
                cout<<"\n CPU burst of process "<<curr_process<<" ended at time "<<curr_time<<".\n";
                p[curr_process].iort=p[curr_process].iobt;
                io.push(curr_process);
                curr_process=-1;
            }
        }
        if(curr_io!=-1)// I/O is executing
        {
            if(p[curr_io].iort==0)//Completion of an I/O
            {
                if(p[curr_io].rt>0)//Completion of intermediate I/O
                {
                    p[curr_io].state=1;
                    p[curr_io].priority=0;
                    cout<<"\n I/O burst of process "<<curr_io<<" ended at time "<<curr_time<<".\n";
                    ready.push(curr_io);
                    curr_io=-1;
                }
                else//Completion of last I/O
                {
                    p[curr_io].state=5;
                    printf("\n -> Process %d Completed at time %d.\n",curr_process,curr_time);
                    p[curr_process].tt=curr_time - p[curr_process].art;
                    curr_io=-1;
                }
            }
            else
            {
                p[curr_io].iort-=1;//Decrement I/O residual time
            }
        }
        if(curr_io==-1)
        {
            if(io.empty()==false)
            {
                curr_io=io.front();
                io.pop();
                cout<<"\n I/O burst of process "<<curr_io<<" began at time "<<curr_time<<".\n";
                p[curr_io].state=3;
                p[curr_io].iort-=1;
            }
        }
        int stopped_process=-1;
        int maxp=-1;
        if(curr_process!=-1)//Current process is not stopped
        {
            if( ready.empty()==false )
            {
                int sq=ready.size();
                int t;
                for(int i=0;i<sq;i++)//Finding process with max priority
                {
                    t=ready.front();
                    if(p[t].priority>maxp)
                    {
                        maxp=p[t].priority;
                    }
                    ready.pop();
                    ready.push(t);
                }
                if(p[curr_process].priority<maxp)
                {
                    cout<<"\n CPU burst of process "<<curr_process<<" ended at time "<<curr_time<<".\n";
                    p[curr_process].state=1;//ready
                    stopped_process=curr_process;
                    curr_process=-1;
                }
                else
                {
                    p[curr_process].rt--;
                }
            }
            else
            {
                p[curr_process].rt--;
            }
        }
        if(curr_process==-1)
        {
            if(ready.empty()==false)
            {
                int process_no;
                int sq=ready.size();
                int t;
                maxp=-1;
                for(int i=0;i<sq;i++)
                {
                    t=ready.front();
                    if(p[t].priority>maxp)
                    {
                        maxp=p[t].priority;
                    }
                    ready.pop();
                    ready.push(t);
                }
                    int flag=0;
                    for(int i=0;i<sq;i++)
                    {
                        t=ready.front();
                        if(p[t].priority==maxp &&flag==0)//Check for 2 or more processes having highest priority
                        {
                            process_no=t;
                            ready.pop();
                            flag=1;
                        }
                        else
                        {
                            ready.pop();
                            ready.push(t);
                        }
                    }
                curr_process=process_no;
                if(stopped_process!=-1)
                {
                    ready.push(stopped_process);
                }
                //cout<<"\n CPU burst of process "<<curr_process<<" began at time "<<curr_time<<".\n";
                p[curr_process].state=2;//running
                //p[curr_process].rt--;
            }
            if(curr_process!=-1)
            {
                cout<<"\n CPU burst of process "<<curr_process<<" began at time "<<curr_time<<".\n";
                cs++;//Incrementing context switch
                if(p[curr_process].bt==p[curr_process].rt)
                {
                    p[curr_process].rest= curr_time - p[curr_process].art;
                }
                p[curr_process].rt--;
            }
        }
        if(curr_process!=-1)
        {
            p[curr_process].priority+=b;
        }
        int sizeq=ready.size();
        for(int i=0;i<sizeq;i++)
        {
            int temp=ready.front();
            ready.pop();
            p[temp].priority+=a;
            ready.push(temp);
        }
        /*
        cout<<"\n Time : "<<curr_time<<endl;
        cout<<"\n Size : "<<ready.size()<<endl;
        for(int i=0;i<n;i++)
        {
            cout<<"\n";
            cout<<" "<<i<<" - ";
            cout<<p[i].priority<<" - "<<p[i].state<<endl;
        }
        cout<<"\n Queue: ";
        int i=0,siq=ready.size();
        while(i<siq)
        {
            int temp=ready.front();
            ready.pop();
            cout<<" "<<temp;
            ready.push(temp);
            i++;
        }
        */
        curr_time++;
    }
    cout<<"\n Number of context switches = "<<cs<<endl;

    for(int i=0;i<n;i++)
    {
        p[i].wt=p[i].tt - p[i].bt;
        cout<<"\n";
        cout<<" For process "<<i<<" :\n\n";
        cout<<" Turnaround Time : "<<p[i].tt<<"\n Response   Time : "<<p[i].rest<<"\n Waiting    Time : "<<p[i].wt<<"\n";
    }
    return 0;
}
