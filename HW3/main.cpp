//
//  main.cpp
//  Maria Parallel Computing HW3
//
//  Created by Xiaoguang Mo on 10/25/14.
//  Copyright (c) 2014 Xiaoguang Mo. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <unistd.h>

using namespace std;

string loc,outloc,global;
string tempFile1="/Users/NNNO/Desktop/temp1.tmp";
string tempFile2="/Users/NNNO/Desktop/temp2.tmp";
string tempFile3="/Users/NNNO/Desktop/temp3.tmp";
pthread_mutex_t amutex = PTHREAD_MUTEX_INITIALIZER;
int AllWorkDone=1,countTempFile=0;
void *Sort(void* temp);

string eraseEnter(string temp){
    for (int i=0; i<temp.length(); i++) {
        if (temp[i]=='\n'||temp[i]=='\r') {
            temp.erase(i,1);
            i-=1;
        }
    }
    return temp;
}

void* ReadFile(void* anyway){
    ifstream fin(loc.c_str());
    int count=1;
    string s,temp;
    while( getline(fin,temp) )
    {
        s=s+temp;
        if (count%1000==0) {
            s=eraseEnter(s);
            pthread_mutex_lock (&amutex);
            global=s;
            pthread_mutex_unlock (&amutex);
            s="";
        }
        count++;
    }
    s=eraseEnter(s);
    pthread_mutex_lock (&amutex);
    global=s;
    pthread_mutex_unlock (&amutex);
    s="";
    sleep(1);
    AllWorkDone=0;
    return NULL;
}

void *Sort(void* temp){
    while (AllWorkDone) {
        if (global.length()>0&&countTempFile<3) {
            ofstream out1;
            out1.open((tempFile1),ios::in);
            if (!out1) {
                outloc=tempFile1;
            }
            ofstream out2;
            out2.open((tempFile2),ios::in);
            if (!out2) {
                outloc=tempFile2;
            }
            ofstream out3;
            out3.open((tempFile3),ios::in);
            if (!out3) {
                outloc=tempFile3;
            }
            pthread_mutex_lock (&amutex);
            sort(global.begin(), global.end());
            pthread_mutex_unlock (&amutex);
            
            ofstream out(outloc.c_str());
            if (out.is_open()) {
                out<<global<<endl;
                out.close();
                pthread_mutex_lock (&amutex);
                countTempFile++;
                pthread_mutex_unlock (&amutex);

            }
            cout<<"global cache's length is :"<<global.length()<<endl;
            pthread_mutex_lock (&amutex);
            global="";
            pthread_mutex_unlock (&amutex);
        }
    }
    return NULL;
}

void* MergeThread(void* temp){
    int sum;
    while (AllWorkDone) {
        cout<<"total file number is :"<<countTempFile<<endl;
        sum=0;
        string readString1,readString2,readString3;
        if (countTempFile>1) {
            //still for mac only
            ifstream in1;
            in1.open((tempFile1),ios::in);
            if (in1) {
                sum-=1;
            }
            in1.close();
            ifstream in3;
            in3.open((tempFile3),ios::in);
            if (in3) {
                sum+=1;
            }
            in3.close();
            sum=-sum;
            if (sum==-1) {
                readString1=tempFile2;
                readString2=tempFile3;
                readString3=tempFile1;
            }
            if (sum==0) {
                readString1=tempFile1;
                readString2=tempFile3;
                readString3=tempFile2;
            }
            if (sum==1) {
                readString1=tempFile1;
                readString2=tempFile2;
                readString3=tempFile3;
            }
            ifstream fin(readString1.c_str());
            string s,temp;
            while( getline(fin,temp) )
            {
                s=s+temp;
            }
            fin.close();
            remove(readString1.c_str());
            cout<<readString1<<" has been removed"<<endl;
            
            ifstream fin2(readString2.c_str());
            string s2,temp2;
            while( getline(fin2,temp2) )
            {
                s2=s2+temp2;
            }
            fin2.close();
            remove(readString2.c_str());
            cout<<readString2<<" has been removed"<<endl;
            pthread_mutex_lock (&amutex);
            countTempFile--;
            pthread_mutex_unlock (&amutex);
            s2=s2+s;
            sort(s2.begin(), s2.end());
            ofstream out(readString3.c_str());
            if (out.is_open()) {
                out<<s2<<endl;
                out.close();
            }

        }
    }
    return NULL;
}

int main(int argc, const char * argv[]) {
    loc="/Volumes/DATA/Downloads/pagecounts-20141001-000000";
    pthread_t thread[3];
    pthread_create(&thread[0], NULL,ReadFile, NULL);
    pthread_create(&thread[1], NULL,Sort,NULL);
    pthread_create(&thread[2], NULL, MergeThread, NULL);
    sleep(100);
    return 0;
}
