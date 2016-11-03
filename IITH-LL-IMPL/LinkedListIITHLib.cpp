/*
*DESCRIPTION :  v2 = here the linked list structure has been changed to accomodate value field as well //{ID|v,nextID}

                The code implements the Linkedlist Datastructure for multithreaded environment, using IITH STM library
*                this implementation will be used to test the performance of IITH STM against the syncrobench.
*                The operations implemented are insert(), delete(), contain()
*                 Insert() : inserts into the linklist
*                Delete() : deletes from the linklist
*                Contain() : finds the given node in a linked list.
*                 printlist(): prints the list.
*AUTHOR      :  AJAY SINGH
*COMPANY     :  IIT Hyderabad
*/
#include <iostream>
#include <pthread.h>
#include <unistd.h> // for sleep function
#include<climits>
#include <math.h>
#include <fstream>

#include <sys/time.h>
#include <time.h>

#define AJBTO

#ifdef AJBTO
#include "IITHSTMCode/BTO.cpp"
#endif
#ifdef AJSGT
#include "IITHSTMCode/SGT.cpp"
#endif
#ifdef AJMVTO
#include "IITHSTMCode/MVTO.cpp"
#endif // AJMVTO

#ifdef AJFOCC
#include "IITHSTMCode/FOCC.cpp"
#endif // AJFOCC

#include <mutex>

using namespace std;

/**********************************************************************************************************************************************/
//  FUNCTION DECLARATION
/**********************************************************************************************************************************************/

void* test(void *threadData);
void print_oblist();
//void* test_add(void *threadData);
//void* test_remove(void *threadData);

/**********************************************************************************************************************************************/
//  MACROS DECLARATION
/**********************************************************************************************************************************************/
#define VAL_MIN  INT_MIN
#define VAL_MAX  INT_MAX
//#define DEBUG 1

/**********************************************************************************************************************************************/
//  GLOBAL DECLARATION
/**********************************************************************************************************************************************/

//node is of form
//{ID|v,nextID}

typedef struct{
int v;              //actual value field, as per it is just the ID only
int nextID;         //ID -given to node while creation
}val_t;

fstream file10runGTOD, file10runCPU, file10runTH, file10runRT, file10runCLOCK;

#ifdef AJBTO
STM* lib = new BTO();
#endif // AJBTO
#ifdef AJSGT
STM* lib = new SGT(1000);
#endif // AJSGT
#ifdef AJMVTO
STM* lib = new MVTO(1000);
#endif // AJMVTO

#ifdef AJFOCC
STM* lib = new FOCC();
#endif // AJFOCC
static int i = 0;
mutex mtx, mtxc;







//Barrier code
typedef struct barrier {
	pthread_cond_t complete;
	pthread_mutex_t mutex;
	int count;
	int crossing;
} barrier_t;

void barrier_init(barrier_t *b, int n)
{
	pthread_cond_init(&b->complete, NULL);
	pthread_mutex_init(&b->mutex, NULL);
	b->count = n;
	cout<<"init barrier "<<n<<endl;
	b->crossing = 0;
}

void barrier_cross(barrier_t *b)
{
	pthread_mutex_lock(&b->mutex);
	/* One more thread through */
	b->crossing++;
	printf("aj crossings %d\n",b->crossing);
	/* If not all here, wait */
	if (b->crossing < b->count) {
		pthread_cond_wait(&b->complete, &b->mutex);
	} else {
		pthread_cond_broadcast(&b->complete);
		/* Reset for next time */
		b->crossing = 0;
	}
	pthread_mutex_unlock(&b->mutex);
}


struct thread_info{
   int  thread_id;
   int thread_val;//value to be inserted for insert operation
   int thread_op;//insert = 0,   delete = 1, lookup = 2
   barrier_t *barrier;
};

int number_of_threads = 1000;
pthread_t *threads ;



//number of delete, insert and lookup threads
int delNum = 0, insertNum = 0, lookupNum = 0;

//default percentage of delete , insert and lookup threads
float delp = 20, insertp = 50, lookp = 30;


//AJ
struct timespec startajcpu, startajth, startajrt, finishajcpu, finishajth, finishajrt;
double elapsedajrt;
double elapsedajcpu;
double elapsedajth, duration;
//clock_t timeaj;
struct timeval start, endaj;
//AJ






int main()
{


    struct thread_info *td; //to store each thread information
    int rc;
    int i;
    void *status;
    int val = 0;

barrier_t barrier;


    /*ALLOCATING MEMORY TO THREADS FOR BOOKEEPING*/
    threads = new pthread_t [number_of_threads];//pthread_t threads[NUMTHREADS];
    td = new struct thread_info [number_of_threads];//struct thread_info td[NUMTHREADS];

    if((threads == nullptr)||(td == nullptr))
    {
        cout<<"error allocating memory";
    }

// openfile to store completion time for plotting performance graphs
    file10runGTOD.open("runGTOD.txt", fstream::out | fstream::app);
    file10runCPU.open("runCPU.txt", fstream::out | fstream::app);
    file10runTH.open("runTH.txt", fstream::out | fstream::app);
    file10runRT.open("runRT.txt", fstream::out | fstream::app);
    file10runCLOCK.open("runCLOCK.txt", fstream::out | fstream::app);

//creating to dummy min and max node of linked list
    if(lib->create_new(VAL_MIN, sizeof(val_t)) != SUCCESS)
    {
       // cout<<"failed to create lid"<<endl;
        return 0;
    }
    if(lib->create_new(VAL_MAX, sizeof(val_t)) != SUCCESS)
    {
       // cout<<"failed to create lid"<<endl;
        return 0;
    }

//inserting above created sentinel id into linked list
{
    long long error_id;
	int read_res=0;
	int write_res=0;
    trans_state* T1 = lib->begin();
    common_tOB* set_obj_min = new common_tOB;
    set_obj_min->size = sizeof(val_t);
    set_obj_min->ID = VAL_MIN;
    read_res = lib->read(T1, set_obj_min);

    common_tOB* set_obj_max = new common_tOB;
    set_obj_max->size = sizeof(val_t);
    set_obj_max->ID = VAL_MAX;
    read_res = lib->read(T1, set_obj_max);

    (*(val_t*)set_obj_min->value).v = VAL_MIN;// assuming v is same as IDs
    (*(val_t*)set_obj_min->value).nextID = VAL_MAX; //= VAL_MAX;//value stores the next field ID , so head node has next fields ID


    (*(val_t*)set_obj_max->value).v = VAL_MAX;
    (*(val_t*)set_obj_max->value).nextID = VAL_MAX;

    write_res=lib->write(T1, set_obj_min);
    write_res=lib->write(T1, set_obj_max);

    if(write_res == SUCCESS)
    {
        lib->try_commit(T1, error_id);
    }
}


print_oblist();
//Now linked list is only two node and this is the initial state of linked list


//Asking user choice of percentage of insert, del, lookup threads  in TODO list

//cout<<"enter % of threads for Addition - "<<endl;
//cin>>insertp;
//cout<<"enter % of threads for deletion - "<<endl;
//cin>>delp;
//cout<<"enter % of threads for lookup   - "<<endl;
//cin>>lookp;

insertNum = (int)ceil((insertp*number_of_threads)/100);
delNum = (int)ceil((delp*number_of_threads)/100);
lookupNum = (int)ceil((lookp*number_of_threads)/100);

//cout<<"#threads"<<insertNum<<" "<<delNum<<" "<<lookupNum<<endl;
//exception verification
if((insertNum + delNum + lookupNum) > number_of_threads)
   {
   cout<<"((insertNum + delNum + lookupNum) > number_of_threads)"<<endl;
    cout<<insertNum<<" "<<delNum<<" "<<lookupNum<<endl;
    return 0;

}


barrier_init(&barrier, number_of_threads +1 );





    /*INITIATING THREADINFO */
   for( i=0; i < number_of_threads; i++ ){
      cout <<"main() : creating thread, " << i << endl;
      td[i].thread_id = i;
      td[i].barrier = &barrier;

      if(i < insertNum)//init threaddata for insert threads
      {
        td[i].thread_op = 0;
        td[i].thread_val = i + 1000;//value of linked list to be inserted
        val = td[i].thread_val;

        //only to insert i need to create new tob ids
        if(lib->create_new(val, sizeof(val_t)) != SUCCESS)
        {
           // cout<<"failed to create lid"<<endl;
            return 0;
        }

      }
      else if(i < (insertNum + delNum ))//init threadData for delete threads
      {
        td[i].thread_op = 1;
        td[i].thread_val = rand()%insertNum + 1000; //rand val within the range of values which are inserted
      }
      else if(i < (insertNum + delNum + lookupNum))//init for lookup threads
      {
        td[i].thread_op = 2;
        td[i].thread_val = rand()%number_of_threads + 1000;
      }
      else
      {
        cout<<"something wrong in thread distribution to operations"<<endl;
      }

//creating threads for insert del and lookup operations
    {
      rc = pthread_create(&threads[i], NULL,
                          test, (void *)&td[i]);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
    }
   }//FOR

   	/* main thread awakes all threads */
	barrier_cross(&barrier);
	cout<<"STARTING..."<<endl;
//init time to log completion time into file

gettimeofday(&start, NULL);

clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startajcpu);
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startajth);
clock_gettime(CLOCK_MONOTONIC_RAW, &startajrt);

double timeaj;
timeaj = clock();


/*WAIT FOR ALL THREADS TO FINISH*/
for( i=0; i < number_of_threads; i++ ){
      rc = pthread_join(threads[i], &status);
      //cout<<"joinin";
      if (rc){
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
   }


//log completion time
gettimeofday(&endaj, NULL);
//CLOCK_MONOTONIC_RAW
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finishajcpu);
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &finishajth);
clock_gettime(CLOCK_MONOTONIC_RAW, &finishajrt);
timeaj = clock() - timeaj;

timeaj = clock() - timeaj;
timeaj = ((double)timeaj)/(CLOCKS_PER_SEC);


cout<<"STOPPING..."<<endl;


duration = (endaj.tv_sec - start.tv_sec);
duration += ( endaj.tv_usec - start.tv_usec)/ 1000000.0;

//	elapsedaj = (finishaj.tv_sec - startaj.tv_sec);
//elapsedaj += (finishaj.tv_nsec - startaj.tv_nsec) / 1000000000.0;

elapsedajcpu = (finishajcpu.tv_sec - startajcpu.tv_sec);
elapsedajcpu += (finishajcpu.tv_nsec - startajcpu.tv_nsec) / 1000000000.0;

elapsedajth = (finishajth.tv_sec - startajth.tv_sec);
elapsedajth += (finishajth.tv_nsec - startajth.tv_nsec) / 1000000000.0;
elapsedajrt = (finishajrt.tv_sec - startajrt.tv_sec);
elapsedajrt += (finishajrt.tv_nsec - startajrt.tv_nsec) / 1000000000.0;




//cout<<"clock() in (s)"<<timeaj<<endl;
cout<<"gettimeofday() in (s)"<<duration<<endl;
cout<<"elapsedajcpu in (s)"<<elapsedajcpu<<endl;
cout<<"elapsedajth in (s)"<<elapsedajth<<endl;
cout<<"elapsedajrt in (s)"<<elapsedajrt<<endl;
printf("clock timeaj %lf ", timeaj);

file10runGTOD<<duration<<endl;
file10runCPU<<elapsedajcpu<<endl;
file10runTH<<elapsedajth<<endl;
file10runRT<<elapsedajrt<<endl;
file10runCLOCK<<timeaj<<endl;

#ifdef DEBUG
    //to safely print the linked list
    mtx.lock();
    print_oblist();
    //cout<<i;
    mtx.unlock();
#endif // DEBUG
  //  cout << "Done!!!" << endl;
    //releasing memory
    delete[] threads;
    delete[] td;

    return 0;
}//main()


/*
*DESCRIPTION    :   Prints the linked list
*AUTHOR         :   AJAY SINGH
*COMPANY        :   IIT Hyderabad
*/
void print_oblist()
{
    int count =0;
    int read_res=0;

    trans_state* T = lib->begin();

    //cout<<"printing..."<<endl;

    //get to head of linked list
    common_tOB* set_obj = new common_tOB;
    set_obj->size = sizeof(val_t);
    set_obj->ID = VAL_MIN;//temp
    lib->read(T, set_obj);
    if(0 != read_res)
    {
        //cout<<"+po:read fail"<<endl;
        //cout<<"+po:killing : "<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    //untill not reached the linked list end s entinel node keep iterating through the list
    while(VAL_MAX != (*(val_t*)set_obj->value).v)
    {
        count++;
        //cout<<(set_obj->ID)<<" --> "<<(*(val_t*)set_obj->value).nextID<<endl;
        set_obj->ID = (*(val_t*)set_obj->value).nextID;
        read_res = lib->read(T, set_obj);
        if(0 != read_res)
        {
            //cout<<"+pol:read fail"<<endl;//pol = print in loop
            //cout<<"+pol:killing : "<<endl;
            //read has failed further execution of thread may cause seg fault
            pthread_exit(NULL);
        }
    }//while

   //cout<<"count - "<<count<<endl;
    delete(set_obj);
}


/*
*DESCRIPTION :  test() executes insert, delete or lookup operation by reading the thread_op of each thread
                if thread_op = 0 --> insert()
                or thread_op = 1 --> delete()
                or thread_op = 2 --> lookup()
*AUTHOR      :  AJAY SINGH
*COMPANY     :  IIT Hyderabad
*REMARK      :  didnot made separate set_add() as threadData was not getting carried and causing crash   TODO
*/
void* test(void *threadData)
{
thread_info *d = (thread_info *)threadData;

/*barrier for all threads otherwise sum inserting threads may arrive later than the main threads finish joining.*/
barrier_cross(d->barrier);

/*......................................................INSERT............................................................................*/
if(d->thread_op == 0)//insert()
 {
	int result, trans_succ_flag;
	long long error_id;
	int read_res=0;
	int write_res=0;
    clock_t time_beg = clock();
//    thread_info *d = (thread_info *)threadData;

	trans_state* T1 = lib->begin();

    //tob to be added
    common_tOB* set_obj = new common_tOB;
    set_obj->size = sizeof(val_t);
    set_obj->ID = d->thread_val;   //each thread will carry the value it wants to insert plus the unique ID of node here in this case both are same
    read_res = lib->read(T1, set_obj);//mem allocated to set_obj->value
    if(0 != read_res)
    {
        //cout<<"+:read fail"<<endl;
        //cout<<"+:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    //since v and id are same so assignin same val to both id and next by default
    (*(val_t*)set_obj->value).v = 0; //default init value of new node
    (*(val_t*)set_obj->value).nextID = VAL_MAX;

//preparing prev and next pointers for list traversal
    common_tOB* set_obj_p = new common_tOB; //prev tob
    set_obj_p->size = sizeof(val_t);
    set_obj_p->ID = VAL_MIN;
    read_res = lib->read(T1, set_obj_p);
    if(0 != read_res)
    {
        //cout<<"+p:read fail"<<endl;
        //cout<<"+p:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    common_tOB* set_obj_n = new common_tOB; //next tob
    set_obj_n->size = sizeof(val_t);
    set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;
    read_res = lib->read(T1, set_obj_n);

    if(0 != read_res)
    {
        //cout<<"+n:read fail"<<endl;
        //cout<<"+n:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

	//linked list is in sorted order of val
	while ((set_obj_n->ID) < d->thread_val)
	{
	   set_obj_p->ID = set_obj_n->ID;
        (*(val_t*)set_obj_p->value).v = (*(val_t*)set_obj_n->value).v; // instead read through the lib API
        (*(val_t*)set_obj_p->value).nextID = (*(val_t*)set_obj_n->value).nextID;

        set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;// prev is equal to next now, (*(val_t*)set_obj_n->value).nextID is equally good.

        read_res = lib->read(T1, set_obj_n);
        if(0 != read_res)
        {
            //  mtx.lock();
            //cout<<"+in while read fail :"<<set_obj_n->ID<<" "<<read_res<<endl;
            //    print_oblist();
            //cout<<"+killing : "<<d->thread_id<<endl;
            //mtx.unlock();
            //read failed no point in continuing transac , do releaselock
            pthread_exit(NULL);//or simply break ;
        }
	}

	if(0 == read_res)
	{
	result = ((set_obj_n->ID) != d->thread_val);

	if (result)//id node not already present insert node
	{

       (*(val_t*)set_obj_p->value).nextID = set_obj->ID;

       (*(val_t*)set_obj->value).nextID = set_obj_n->ID;

        write_res=lib->write(T1, set_obj);
        if(0 != write_res)
        {
            printf("+write fail");
        }
        write_res=lib->write(T1, set_obj_p);
        if(0 != write_res)
        {
            printf("+write fail");
        }
        write_res=lib->write(T1, set_obj_n);
        if(0 != write_res)
        {
            printf("+write fail");
        }

	}
    trans_succ_flag = lib->try_commit(T1, error_id);
    if(0 != trans_succ_flag)
    {
       // cout<<"+commit failed "<<d->thread_id<<endl;
    }

    //release memory of temporary objects created after transactions commit

	delete(set_obj);
	delete(set_obj_p);
	delete(set_obj_n);
}//if(0 == read_res)
}//if(d->thread_id < insertNum)//insert()

/*......................................................INSERT END............................................................................*/

/*......................................................DELETE................................................................................*/
else if(d->thread_op == 1)//(d->thread_id < insertNum + delNum)//delete()
{
	int result, trans_succ_flag;;
	long long error_id;
	int read_res=0;
	int write_res=0;

    int v;

    trans_state* T = lib->begin();

    common_tOB* set_obj_p = new common_tOB; //prev tob
    set_obj_p->size = sizeof(val_t);
    set_obj_p->ID = VAL_MIN;
    read_res = lib->read(T, set_obj_p);
    if(0 != read_res)
    {
        //cout<<"-p:read fail"<<endl;
        //cout<<"-p:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    common_tOB* set_obj_n = new common_tOB; //next tob
    set_obj_n->size = sizeof(val_t);
    set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;
    read_res = lib->read(T, set_obj_n);
    if(0 != read_res)
    {
       // cout<<"-n:read fail"<<endl;
       // cout<<"-n:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    while(1)
    {
        v = (*(val_t*)set_obj_p->value).nextID;
        if (v >= d->thread_val)
            break;//node is found or cannot be found by further iterations sincle list is in sorted oreder

        set_obj_p->ID = set_obj_n->ID;
        (*(val_t*)set_obj_p->value).v = (*(val_t*)set_obj_n->value).v; // instead read through the lib API
        (*(val_t*)set_obj_p->value).nextID = (*(val_t*)set_obj_n->value).nextID;

        set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;

        read_res = lib->read(T, set_obj_n);
        if(0 != read_res)
        {
           // cout<<"-p:in while read fail"<<endl;
           // cout<<"-p:killing : "<<d->thread_id<<endl;
            //read has failed further execution of thread may cause seg fault
            pthread_exit(NULL);
        }

    }

    //currently unique ID and val of node are same
    result = (v == d->thread_val);//value to be deleted is to be supplied by thread

    if (result)//means next obj is to be deleted
    {

        (*(val_t*)set_obj_p->value).nextID = (*(val_t*)set_obj_n->value).nextID; // node to be deleted is skipped
        write_res=lib->write(T, set_obj_p);
        if(0 != write_res)
        {
            printf("-write fail");
        }

        write_res=lib->write(T, set_obj_n);
        if(0 != write_res)
        {
            printf("-write fail");
        }

    }
    #ifdef DEBUG
    else
    {
        cout<<"not found!cannt be deleted"<<endl;
    }
    #endif // DEBUG

    trans_succ_flag = lib->try_commit(T, error_id);
    if(0 != trans_succ_flag)
    {
       // cout<<"-commit failed "<<d->thread_id<<endl;
    }
    delete(set_obj_p);
	delete(set_obj_n);

}//remove
/*......................................................DELETE END............................................................................*/

/*......................................................LOOKUP............................................................................*/
else if(d->thread_op == 2)//contain
{
    int result, trans_succ_flag;;
	long long error_id;
	int read_res=0;
	int write_res=0;

    int v;

    trans_state* T = lib->begin();

    common_tOB* set_obj_p = new common_tOB; //prev tob
    set_obj_p->size = sizeof(val_t);
    set_obj_p->ID = VAL_MIN;
    read_res = lib->read(T, set_obj_p);
    if(0 != read_res)
    {
        //cout<<"*p:read fail"<<endl;
        //cout<<"*p:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    common_tOB* set_obj_n = new common_tOB; //next tob
    set_obj_n->size = sizeof(val_t);
    set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;
    read_res = lib->read(T, set_obj_n);
    if(0 != read_res)
    {
       // cout<<"*n:read fail"<<endl;
       // cout<<"*n:killing : "<<d->thread_id<<endl;
        //read has failed further execution of thread may cause seg fault
        pthread_exit(NULL);
    }

    while(1)
    {
        v = (*(val_t*)set_obj_p->value).nextID;
        if (v >= d->thread_val)// as per now unique id of node and val of node are same so using threadVal casually
            break;//node is found or cannot be found anymore

        set_obj_p->ID = set_obj_n->ID;
        (*(val_t*)set_obj_p->value).v = (*(val_t*)set_obj_n->value).v; // instead read through the lib API
        (*(val_t*)set_obj_p->value).nextID = (*(val_t*)set_obj_n->value).nextID;

        set_obj_n->ID = (*(val_t*)set_obj_p->value).nextID;

        read_res = lib->read(T, set_obj_n);
        if(0 != read_res)
        {
           // cout<<"-p:in while read fail"<<endl;
         //   cout<<"-p:killing : "<<d->thread_id<<endl;
            //read has failed further execution of thread may cause seg fault
            pthread_exit(NULL);
        }

    }
    #ifdef DEBUG
    if(v == d->thread_val)
    {
        cout<<"found yeah "<<d->thread_val<<endl;
    }
    else{
    cout<<"not found..."<<endl;
    }
    #endif // DEBUG

}

/*......................................................LOOKUP END............................................................................*/

//    mtxc.lock();
//    cout<< d->thread_id<<"exiting"<<endl;
//    mtxc.unlock();

   pthread_exit(NULL);
}

