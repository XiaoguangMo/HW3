//
//  main.cpp
//  HW3
//
//  Created by NNNO on 10/25/14.
//  Copyright (c) 2014 NNNO. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

string loc;
string outloc="/Users/NNNO/Desktop/testing3.tmp";
string tempFile1="/Users/NNNO/Desktop/temp1.tmp";
string tempFile2="/Users/NNNO/Desktop/temp2.tmp";
string tempFile3="/Users/NNNO/Desktop/temp3.tmp";
string tempFile4="/Users/NNNO/Desktop/temp4.tmp";
string tempFile5="/Users/NNNO/Desktop/temp5.tmp";
string global;
char FileNeedMerge;
int AllWorkDone=1;

void MergeSort(char* array, int left, int right);
void Merge(char* array, int left, int mid, int right);
void *Sort(void* temp);
//归并排序

int CountLines(string filename)
{
    ifstream fin(loc.c_str());
    int count;
    string s;
    while( getline(fin,s) )
    {
        count++;
    }
    return count;
}

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
            cout<<"get input at count:"<<count<<endl;
            s=eraseEnter(s);
            global=s;
            s="";
        }
        count++;
    }
    s=eraseEnter(s);
    global=s;
    return NULL;
}

void *Sort(void* temp){
    while (AllWorkDone) {
        if (global.length()>0) {
//            cout<<"start input"<<endl;
            sort(global.begin(), global.end());
            
            ofstream out1;
            out1.open((tempFile2),ios::in);
            
            if (!out1) {
                cout<<"no"<<endl;
            }else{
                cout<<"yes"<<endl;
            }
            ofstream out(outloc.c_str());
            if (out.is_open()) {
                out<<global<<endl;
//                out<<"hello"<<endl;
//                for (int i=0; i<5; i++) {
//                    if (FileNeedMerge[i][0]!='\0') {
//                        for (int j=0; j<outloc.length(); j++) {
//                            FileNeedMerge[i][j]=outloc[j];
//                        }
//                    }
//                }
                out.close();
            }
            global="";
//            cout<<"global is :"<<global<<endl;
        }
    }
    return NULL;
}

void MergeSort(char* array, int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;
        MergeSort(array, left, mid);
        MergeSort(array, mid + 1, right);
        Merge(array, left, mid, right);
    }
}
//合并两个已排好序的子链

void Merge(char* array, int left, int mid, int right)
{
    char* temp = new char[right - left + 1];
    int i = left, j = mid + 1, m = 0;
    while(i <= mid && j <= right)
    {
        if(array[i] < array[j])
        {
            temp[m++] = array[i++];
        }
        else
        {
            temp[m++] = array[j++];
        }
    }
    while(i <= mid)
    {
        temp[m++] = array[i++];
    }
    while(j <= right)
    {
        temp[m++] = array[j++];
        
    }
    for(int n = left, m = 0; n <= right; n++, m++)
    {
        array[n] = temp[m];
    }
    delete temp;
}

int main(int argc, const char * argv[]) {
    loc="/Users/NNNO/Desktop/testing2.txt";
//    int lines=CountLines(loc.c_str());
//    int blocks=ceil(lines/1000);
    pthread_t thread[3];
    pthread_create(&thread[0], NULL,ReadFile, NULL);
    pthread_create(&thread[1], NULL,Sort,NULL);
    sleep(3);
//    pthread_create(<#pthread_t *#>, <#const pthread_attr_t *#>, <#void *(*)(void *)#>, <#void *#>)
//    ReadFile(loc.c_str());
    return 0;
}
