#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<queue>
#include<fstream>

using namespace std;


struct process
{
        char process_id; //process id
        int arrival_time; //arrival time
        int burst_time; //CPU burst time
        int burst_count; //Total number of bursts to be executed
        int io_burst_time; //IO burst time
        bool inuse; //to tell if a process is in use
        bool io_use; //to tell if a process is in io use
        bool completed;
        int endTime; // turnaround time for that process
        int wt; // wait time for that process
        bool arrived;
        int current_cpu_burst; //current cpu burst
        int current_io_burst; // current io burst
};


void init(struct process *p, char *line[], int n) //initialize the lines into processes
{
    int i;
  //  int j;
    int count;
    std:: cout<<"Init called\n";
    
    for(i=0; i<n;++i)
    {
        count=0;
        p[i].inuse = false;
        p[i].completed = false;
        p[i].wt = 0;
        p[i].arrived = false;
        char *token=strtok(line[i],"|");
        while(token!=NULL)
        {
            if(count==0)
            {
                p[i].process_id=line[i][0];
            }
            else if(count==1)
            {
                p[i].arrival_time = atoi(token);
            }
            else if(count==2)
            {
                p[i].burst_time = atoi(token);
            }
            else if(count==3)
            {
                p[i].burst_count = atoi(token);
            }
            else if(count==4)
            {
                p[i].io_burst_time = atoi(token);
            }
            ++count;
            token=strtok(NULL,"|");
        }
        free(token);
    }
}

/*
int FIFO(struct process *p, int n, int t_cs)
{
    queue <struct process*> ready_queue;
    queue <struct process*> io_queue;
    queue <struct process*> temp_queue;
    
    struct process *CPU = NULL;
    struct process *IO = NULL;

    int completed = 0;
    int clock = 0;
    int cpu_tcs = t_cs / 2;
    int io_tcs = t_cs / 2;
   
    while(completed<n)
    {
    	for(int i=0; i<n; ++i) // add all the incoming processes to the temporary queue
    	{
    		if(p[i].arrival_time<=clock && p[i].arrived==false)
    		{
    			p[i].arrived = true;
    			temp_queue.push(&p[i]);
    		}
    	}
    	
        // !------------------ Code for emptying the temporary queue----- sorting ------ adding to ready queue ----- PHEW !! 
        struct process *p_temp[n];
        int count = 0;
        
        while(!temp_queue.empty())
        {
            p_temp[count] = temp_queue.front();
            temp_queue.pop();
            ++count;
        }
        
        for(int i=0; i<count; ++i)
        {
            for(int j=i+i; j<count; ++j)
            {
                struct process *temp;
                if(p_temp[i]->process_id > p_temp[j]->process_id)
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
            }
        }
        
        for(int i=0; i<count; ++i)
        {
            ready_queue.push(p_temp[i]);
        }
        
        // !------------------ Yaaaaaaaayyyyyyyyeeeeeee -----------------
        
        if(ready_queue.empty())
        {
            cpu_tcs = t_cs/2;
        }
        
        if(CPU==NULL && ready_queue.size()>0) // add a process to CPU from the ready queue
        {
            if(cpu_tcs>0)
            {
                --cpu_tcs;
            }
            else if(cpu_tcs==0)
            {
        	    	CPU = ready_queue.front();
        	    	cout << "time "<<clock<<"ms: Process "<<CPU->process_id<<" started using the CPU\n";
        	    	ready_queue.pop();
        	    	cpu_tcs = t_cs / 2;
            }
        }
        
        
        if(CPU!=NULL)  // add the process from CPU to the IO queue
        {
            if((CPU->current_cpu_burst==CPU->burst_time))
            {
                if(io_tcs>0)
                {
                --io_tcs;
                }
                else if(io_tcs==0)
                {
                    --cpu_tcs;
                    CPU->endTime = clock - (t_cs/2);
                    CPU->burst_count--;
                    if(CPU->burst_count>0)
                    {
                        cout <<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" switched out of CPU; will block on I/O until time "<<CPU->io_burst_time<<"ms\n";
                        CPU->current_cpu_burst=0;
                        io_queue.push(CPU);
                    }
                    else if(CPU->burst_count==0)
                    {
                        cout<<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" terminated\n";
                        CPU->completed = true;
                        ++completed;
                    }
                CPU = NULL;
                io_tcs = t_cs / 2;
                }    
            }
            else
            {
                CPU->current_cpu_burst++;
            }
        }
        int size = io_queue.size();
        struct process *io;
        
        // updating the IO queue -----------------------------
        for(int i=0; i<size; ++i)
        {
            io = io_queue.front();
            ++io->current_io_burst;
            if(io->current_io_burst == io->io_burst_time)
            {
                cout<<"time "<<clock<<"ms: Process "<<io->process_id<<" completed I/O, added to ready queue\n";
                io->current_io_burst=0;
                temp_queue.push(io);
                io_queue.pop();
            }
            else
            {
                io_queue.pop();
                io_queue.push(io);
            }
        }
        // ! ---------------- IO queue updated yipeeeeeeeeeeeeeeeeee -----------
        ++clock;
        
    }
    cout<<"time "<<--clock<<"ms: Simulator ended for FCFS\n";
return EXIT_SUCCESS;
}

int RR(struct process *p, int n, int t_cs, int t_slice)
{
    queue <struct process*> ready_queue;
    queue <struct process*> io_queue;
    queue <struct process*> temp_queue;
    
    struct process *CPU = NULL;
    struct process *IO = NULL;

    int completed = 0;
    int clock = 0;
    int cpu_tcs = t_cs / 2;
    int io_tcs = t_cs / 2;
    int slice = t_slice;
    int rr_tcs = t_cs / 2;
   
    while(completed<n)
    {
    	for(int i=0; i<n; ++i) // add all the incoming processes to the temporary queue
    	{
    		if(p[i].arrival_time<=clock && p[i].arrived==false)
    		{
    			p[i].arrived = true;
    			temp_queue.push(&p[i]);
    		}
    	}
    	
        // !------------------ Code for emptying the temporary queue----- sorting ------ adding to ready queue ----- PHEW !! 
        struct process *p_temp[n];
        int count = 0;
        
        while(!temp_queue.empty())
        {
            p_temp[count] = temp_queue.front();
            temp_queue.pop();
            ++count;
        }
        
        for(int i=0; i<count; ++i)
        {
            for(int j=i+i; j<count; ++j)
            {
                struct process *temp;
                if(p_temp[i]->process_id > p_temp[j]->process_id)
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
            }
        }
        
        for(int i=0; i<count; ++i)
        {
            ready_queue.push(p_temp[i]);
        }
        
        // !------------------ Yaaaaaaaayyyyyyyyeeeeeee -----------------
        
        if(ready_queue.empty())
        {
            cpu_tcs = t_cs/2;
        }
        
        if(CPU==NULL && ready_queue.size()>0) // add a process to CPU from the ready queue
        {
            if(cpu_tcs>0)
            {
                --cpu_tcs;
            }
            else if(cpu_tcs==0)
            {
        	    	CPU = ready_queue.front();
        	    	cout << "time "<<clock<<"ms: Process "<<CPU->process_id<<" started using the CPU\n";
        	    	ready_queue.pop();
        	    	cpu_tcs = t_cs / 2;
            }
        }
        
        
        if(CPU!=NULL)  // add the process from CPU to the IO queue
        {
            if((CPU->current_cpu_burst==CPU->burst_time) && slice>=0)
            {
                if(io_tcs>0)
                {
                --io_tcs;
                }
                else if(io_tcs==0)
                {
                    --cpu_tcs;
                    CPU->endTime = clock - (t_cs/2);
                    CPU->burst_count--;
                    if(CPU->burst_count>0)
                    {
                        cout <<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" switched out of CPU; will block on I/O until time "<<CPU->io_burst_time<<"ms\n";
                        CPU->current_cpu_burst=0;
                        io_queue.push(CPU);
                    }
                    else if(CPU->burst_count==0)
                    {
                        cout<<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" terminated\n";
                        CPU->completed = true;
                        ++completed;
                    }
                CPU = NULL;
                slice = t_slice;
                io_tcs = t_cs / 2;
                }
            }
            else if((CPU->current_cpu_burst < CPU->burst_time) && slice==0 && ready_queue.size()>0)
                {
                    if(rr_tcs>0)
                    {
                        --rr_tcs;
                    }
                    else if(rr_tcs==0)
                    {
                        temp_queue.push(CPU);
                        slice = t_slice;
                        CPU = NULL;
                        --cpu_tcs;
                        rr_tcs = t_cs / 2;
                    }
                
                }
            else if((CPU->current_cpu_burst < CPU->burst_time) && slice==0 && ready_queue.size()==0) // round robin continues since ready queue is empty
            {
                slice = t_slice;
                --slice;
                CPU->current_cpu_burst++;
            }
            else 
            {
                CPU->current_cpu_burst++;
                --slice;
            }
        }
        int size = io_queue.size();
        struct process *io;
        
        // updating the IO queue -----------------------------
        for(int i=0; i<size; ++i)
        {
            io = io_queue.front();
            ++io->current_io_burst;
            if(io->current_io_burst == io->io_burst_time)
            {
                cout<<"time "<<clock+1<<"ms: Process "<<io->process_id<<" completed I/O, added to ready queue\n";
                io->current_io_burst=0;
                temp_queue.push(io);
                io_queue.pop();
            }
            else
            {
                io_queue.pop();
                io_queue.push(io);
            }
        }
        // ! ---------------- IO queue updated yipeeeeeeeeeeeeeeeeee -----------
        ++clock;
        
    }
    cout<<"time "<<--clock<<"ms: Simulator ended for Round Robin\n";
return EXIT_SUCCESS;
}

struct comparison // sorting the priority queue
{
    bool operator()(struct process *p1, struct process *p2)
    {
        if((p1->burst_time - p1->current_cpu_burst) < (p2->burst_time - p2->current_cpu_burst))
        {
            return true;
        }
        return false;
    }
};
*/
int SRT(struct process *p, int n, int t_cs)
{
    queue <struct process*> temp_queue;
    queue <struct process*> ready_queue;
    queue <struct process*> io_queue;
    
    struct process *CPU = NULL;
    struct process *IO = NULL;
    struct process *switch_cpu = NULL;
    struct process *frontcpu = NULL;

    int sw=0;
    int completed = 0;
    int clock = 0;
    int cpu_tcs = t_cs / 2;
    int io_tcs = t_cs / 2;
    int update = 0;

    while(completed<n)
    {
        for(int i=0; i<n; ++i) // add all the incoming processes to the temporary queue
    	{
    		if(p[i].arrival_time<=clock && p[i].arrived==false)
    		{
    			p[i].arrived = true;
    			temp_queue.push(&p[i]);
    		}
    	}
    	struct process *p_temp[n];
        int count = 0;
        
        while(!temp_queue.empty())
        {
            p_temp[count] = temp_queue.front();
            temp_queue.pop();
            ++count;
        }
        
        for(int i=0; i<count; ++i)
        {
            for(int j=i+i; j<count; ++j)
            {
                struct process *temp;
                if((p_temp[i]->burst_time - p_temp[i]->current_cpu_burst) > (p_temp[j]->burst_time - p_temp[j]->current_cpu_burst))
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
                else if((p_temp[i]->burst_time - p_temp[i]->current_cpu_burst) == (p_temp[j]->burst_time - p_temp[j]->current_cpu_burst) && (p_temp[i]->process_id > p_temp[j]->process_id))
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
            }
        }
        for(int i=0; i<count; ++i)
        {
            ready_queue.push(p_temp[i]);
        }
        count=0;
 /*       while(!ready_queue.empty())
        {
            p_temp[count] = ready_queue.front();
            ready_queue.pop();
            ++count;
        }
        for(int i=0; i<count; ++i)
        {
            for(int j=i+i; j<count; ++j)
            {
                struct process *temp;
                if((p_temp[i]->burst_time - p_temp[i]->current_cpu_burst) > (p_temp[j]->burst_time - p_temp[j]->current_cpu_burst))
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
                else if((p_temp[i]->burst_time - p_temp[i]->current_cpu_burst) == (p_temp[j]->burst_time - p_temp[j]->current_cpu_burst) && (p_temp[i]->process_id > p_temp[j]->process_id))
                {
                    temp = p_temp[j];
                    p_temp[j] = p_temp[i];
                    p_temp[i] = temp;
                }
            }
        }
        for(int i=0; i<count; ++i)
        {
            ready_queue.push(p_temp[i]);
        }
    */
        if(ready_queue.empty())
        {
            cpu_tcs = t_cs/2;
        }
        
        if(CPU==NULL && ready_queue.size()>0) // add a process to CPU from the ready queue
        {
            if(cpu_tcs == (t_cs/2) - 1)
            {
                switch_cpu = ready_queue.front(); // store the process at the beginning of ready queue in context switch time
            }
            if(cpu_tcs>0)
            {
                --cpu_tcs;
            }
            else if(cpu_tcs==0)
            {
                    frontcpu = ready_queue.front(); // check the first element at end of context switch
                    if(frontcpu == switch_cpu)
                    {
                        CPU = ready_queue.front();
        	    	    cout << "time "<<clock<<"ms: Process "<<CPU->process_id<<" started using the CPU [Q ";
        	    	    ready_queue.pop();
        	    	    int c=0;
        	    	    struct process *display[n];
        	    	    while(!ready_queue.empty())
        	    	    {
        	    	        display[c] = ready_queue.front();
        	    	        cout<<display[c]->process_id<<" ";
        	    	        ++c;
        	    	        ready_queue.pop();
        	    	    }
        	    	    cout<<"]\n";
        	    	    for(int i=0; i<c;++i)
        	    	    {
        	    	        ready_queue.push(display[i]);
        	    	    }
        	    	    cpu_tcs = t_cs / 2;    
        	    	    frontcpu = NULL;
        	    	    switch_cpu = NULL;
                    }
                    else
                    {
                        cout <<"time "<<clock<<"ms: Clock is being updated by 3 ms "<<switch_cpu->process_id <<"    "<<frontcpu->process_id<<"\n\n\n\n\n\n";
                        CPU = NULL;
                        clock+=3;
                        update = 3;
                        
                        cpu_tcs = t_cs / 2 ;
                    }
        	 }
        }
        if(CPU!=NULL)  // add the process from CPU to the IO queue
        {
            struct process *front = NULL;
            if(ready_queue.size()>0)
            {
                front = ready_queue.front();    
            }
            if(front!=NULL)
            {
                if((CPU->burst_time - CPU->current_cpu_burst) > (front->burst_time - front->current_cpu_burst)) 
                {
                cout <<"time "<<clock<<"ms: clock is about to be updated by 3 ms "<<CPU->process_id<<"      "<<front->process_id<<"\n";
                clock+=3;
                update = 3;
                temp_queue.push(CPU);
                CPU = NULL;
                goto end_of_process;
                 }
            }
            if((CPU->current_cpu_burst==CPU->burst_time))
            {
               
                if(io_tcs>0)
                {
                --io_tcs;
                }
                else if(io_tcs==0)
                {
                    --cpu_tcs;
                    CPU->endTime = clock - (t_cs/2);
                    CPU->burst_count--;
                    if(CPU->burst_count>0)
                    {
                        cout <<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" switched out of CPU; will block on I/O until time "<<CPU->io_burst_time<<"ms [Q ";
                        int c=0;
        	    	    struct process *display[n];
        	    	    while(!ready_queue.empty())
        	    	    {
        	    	        display[c] = ready_queue.front();
        	    	        cout<<display[c]->process_id<<" ";
        	    	        ++c;
        	    	        ready_queue.pop();
        	    	    }
        	    	    cout<<"]\n";
        	    	    for(int i=0; i<c;++i)
        	    	    {
        	    	        ready_queue.push(display[i]);
        	    	    }
                        CPU->current_cpu_burst=0;
                        io_queue.push(CPU);
                    }
                    else if(CPU->burst_count==0)
                    {
                        cout<<"time "<<CPU->endTime<<"ms: Process "<<CPU->process_id<<" terminated [Q ";
                        int c=0;
        	    	    struct process *display[n];
        	    	    while(!ready_queue.empty())
        	    	    {
        	    	        display[c] = ready_queue.front();
        	    	        cout<<display[c]->process_id<<" ";
        	    	        ++c;
        	    	        ready_queue.pop();
        	    	    }
        	    	    cout<<"]\n";
        	    	    for(int i=0; i<c;++i)
        	    	    {
        	    	        ready_queue.push(display[i]);
        	    	    }
                        CPU->completed = true;
                        ++completed;
                    }
                   
                CPU = NULL;
                io_tcs = t_cs / 2;
                }    
            }
            else
            {
                CPU->current_cpu_burst++;
            }
        }
    end_of_process:
    
        int size = io_queue.size();
        struct process *io;
        
        // updating the IO queue -----------------------------
        while(update>-1)
        {
        for(int i=0; i<size; ++i)
        {
                io = io_queue.front();
                ++io->current_io_burst;
                if(io->current_io_burst == io->io_burst_time)
                {
                cout<<"time "<<clock+1<<"ms: Process "<<io->process_id<<" completed I/O, added to ready queue [Q ";
                io->current_io_burst=0;
                temp_queue.push(io);
                io_queue.pop();
                int c=0;
        	    	    struct process *display[n];
        	    	    while(!ready_queue.empty())
        	    	    {
        	    	        display[c] = ready_queue.front();
        	    	        cout<<display[c]->process_id<<" ";
        	    	        ++c;
        	    	        ready_queue.pop();
        	    	    }
        	    	    cout<<"]\n";
        	    	    for(int i=0; i<c;++i)
        	    	    {
        	    	        ready_queue.push(display[i]);
        	    	    }
               }
               else
                  {
                    io_queue.pop();
                    io_queue.push(io);
                    }
        }
        --update;
        }
    update = 0;
    ++clock;
    }
}










int main(int argc, char **argv)
{
    int t_cs=8; //context switch time
    int i;
    FILE *fptr;
    char *line[26]; //assumed that the total length of characters in a line won't be more than 52
    int t_slice = 80;
    ssize_t read;
    size_t len = 0;
    int n=0;
    if(argc!=2)
    {
        perror("ERROR: Invalid Number of Arguments\n");
        return EXIT_FAILURE;
    }
    fptr=fopen(argv[1],"r");
    
    for(i=0; i<26; ++i)
    {
        line[i] = (char *)new char[50];
    }
    while ((read = getline(&line[n], &len, fptr)) != -1)
    {
        if(line[n][0]!='#')
        {
            if(line[n][read-1]=='\n')
                line[n][read-1]='\0';
            ++n;
        }
        
    }
   
    for(i=0;i<n;++i)
    {
         std:: cout<< line[i]<<"\n";
    }
    struct process *p = new process[n];
    
    
    init(p, line,n); //initialise the processes into struct p 
    
 //   int a = FIFO(p, n, t_cs);
    
 //   delete(p);
    
 //   struct process *p = new process[n];
    
 //   init(p, line, n);
    
//    int b = RR(p, n, t_cs, t_slice);
  
    int c = SRT(p, n, t_cs);  
    return EXIT_SUCCESS;
}






