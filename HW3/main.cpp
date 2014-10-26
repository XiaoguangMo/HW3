//
//  main.cpp
//  HW3
//
//  Created by NNNO on 10/25/14.
//  Copyright (c) 2014 NNNO. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <unistd.h>

using namespace std;

string loc;
string outloc="/Users/NNNO/Desktop/testing3.tmp";
string tempFile1="/Users/NNNO/Desktop/temp1.tmp";
string tempFile2="/Users/NNNO/Desktop/temp2.tmp";
string tempFile3="/Users/NNNO/Desktop/temp3.tmp";
//string tempFile4="/Users/NNNO/Desktop/temp4.tmp";
//string tempFile5="/Users/NNNO/Desktop/temp5.tmp";
string global;
pthread_mutex_t amutex = PTHREAD_MUTEX_INITIALIZER;
//char FileNeedMerge;
int AllWorkDone=1;
int countTempFile=0;

void *Sort(void* temp);
string MergeSort(string str, int n);
//归并排序


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
//    cout<<block<<endl;
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
//    cout<<global<<endl;
//    AllWorkDone=0;
    return NULL;
}

void *Sort(void* temp){
//    int countcount=0;
    while (AllWorkDone) {
        if (global.length()>0&&countTempFile<3) {
//            if (global.length()>0) {
//            countcount++;
//            sort(global.begin(), global.end());
            
            //
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
//                global=MergeSort(global, (int)global.length());
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

            
//            cout<<"global is :"<<global.length()<<endl;
        }
//    cout<<countcount<<endl;

    }
    return NULL;
}

void* MergeThread(void* temp){
//    int tempor[]={-1,0,1};
    int sum;
    while (AllWorkDone) {
        cout<<"total file number is :"<<countTempFile<<endl;
//        int count=0;
        sum=0;
        string readString1,readString2,readString3;
        if (countTempFile>1) {
//            cout<<countTempFile<<endl;
            //still for mac only
            ifstream in1;
            in1.open((tempFile1),ios::in);
            if (in1) {
//                readString1=tempFile1;
                sum-=1;
            }
            in1.close();
//            ifstream in2;
//            in2.open((tempFile2),ios::in);
//            if (in2) {
//            }
//            in2.close();
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
//                cout<<s2<<endl;
            }
            fin2.close();
            remove(readString2.c_str());
            cout<<readString2<<" has been removed"<<endl;

            pthread_mutex_lock (&amutex);
            countTempFile--;
            pthread_mutex_unlock (&amutex);
//            cout<<s.length()<<" "<<s2.length()<<endl;
            s2=s2+s;
//            s2=MergeSort(s2, (int)s2.length());
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

string MergeSort(string str, int n)
{
    //If the string is only one
    //character then it is already
    //sorted
    if (n == 1)
    {
        return str;
    }
    else
    {
        //Calculate the middle
        int m = n/2;
        
        string a=str.substr(0,m);
        string b=str.substr(m,n-m);
        //Recursively call the function on the two
        //halves
        a = MergeSort(a, m);
        b = MergeSort(b, n-m);
        
        //The rest of the code is to merge the sorted
        //halves
        int l = 0;
        int r = 0;
        int k = 0;
        
        while (l < m && r < n-m)
        {
            //If the character from the first half
            //is less than the character of the
            //second half then push it to the result
            if (a[l] <= b[r])
            {
                str[k] = a[l];
                l++;
            }
            //Otherwise push the character from the
            //second half
            else
            {
                str[k] = b[r];
                r++;
            }
            
            k++;
        }
        
        //If the loop above is finished and still
        //l is less than m then this means there
        //are characters in the first half that
        //need to be appended to the result
        if (l < m)
        {
            while (l < m)
            {
                str[k] = a[l];
                l++;
                k++;
            }
        }
        
        //Similarly we need to append those characters
        //in the second half that were not added
        if (r < n-m)
        {
            while( r < n-m)
            {
                str[k] = b[r];
                r++;
                k++;
            }
        }
        
        //Return the sorted string
        return str;
    }
}

int main(int argc, const char * argv[]) {
//    loc="/Users/NNNO/Desktop/testing2.txt";
    loc="/Volumes/DATA/Downloads/pagecounts-20141001-000000";
//    ofstream out(loc.c_str());
//    if (out.is_open()) {
//        for (int i=0; i<4500; i++) {
//            out<<i<<endl;
//        }
//        out.close();
//    }
    pthread_t thread[3];
    pthread_create(&thread[0], NULL,ReadFile, NULL);
    pthread_create(&thread[1], NULL,Sort,NULL);
    pthread_create(&thread[2], NULL, MergeThread, NULL);
    sleep(3000);
    

    return 0;
}
