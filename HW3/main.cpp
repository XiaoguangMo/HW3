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
string tempFile4="/Users/NNNO/Desktop/temp4.tmp";
string tempFile5="/Users/NNNO/Desktop/temp5.tmp";
string global;
char FileNeedMerge;
int AllWorkDone=1;
int countTempFile=0;

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
            s=eraseEnter(s);
            global=s;
            s="";
        }
        count++;
    }
    s=eraseEnter(s);
    global=s;
    cout<<global<<endl;
    return NULL;
}

void *Sort(void* temp){
    int countcount=0;
while (AllWorkDone) {
        if (global.length()>0) {
            countcount++;
            sort(global.begin(), global.end());
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
            ofstream out4;
            out4.open((tempFile4),ios::in);
            if (!out4) {
                outloc=tempFile4;
            }
            ofstream out5;
            out5.open((tempFile5),ios::in);
            if (!out5) {
                outloc=tempFile5;
            }
            
            
            ofstream out(outloc.c_str());
            if (out.is_open()) {
                out<<global<<endl;
                out.close();
                countTempFile++;
            }
            global="";
//            cout<<"global is :"<<global<<endl;
        }
//    cout<<countcount<<endl;

    }
    return NULL;
}

void* MergeThread(void* temp){
    while (AllWorkDone) {
//        cout<<"total file number is :"<<countTempFile<<endl;
//        int count=0;
        string readString1,readString2;
        if (countTempFile>1) {
//            cout<<countTempFile<<endl;
            //still for mac only
            ifstream in1;
            in1.open((tempFile1),ios::in);
            if (in1) {
                readString1=tempFile1;
            }
            in1.close();
            ifstream in2;
            in2.open((tempFile2),ios::in);
            if (in2) {
                readString1=tempFile2;
            }
            in2.close();
            ifstream in3;
            in3.open((tempFile3),ios::in);
            if (in3) {
                readString1=tempFile3;
            }
            in3.close();
            ifstream in4;
            in4.open((tempFile4),ios::in);
            if (in4) {
                readString1=tempFile4;
            }
            in4.close();
            ifstream in5;
            in5.open((tempFile5),ios::in);
            if (in5) {
                readString1=tempFile5;
            }
            in5.close();
            
            ifstream fin(readString1.c_str());
            string s,temp;
            while( getline(fin,temp) )
            {
                s=s+temp;
            }
            fin.close();
            remove(readString1.c_str());
//            cout<<readString1<<" has been removed"<<endl;
            
//            string readSt
//            ifstream in1;
            ifstream in6;
            ifstream in7;
            ifstream in8;
            ifstream in9;
            ifstream in10;
            in6.open((tempFile1),ios::in);
            if (in6) {
                readString2=tempFile1;
            }
            in6.close();
            in7.open((tempFile2),ios::in);
            if (in7) {
                readString2=tempFile2;
            }
            in7.close();
            in8.open((tempFile3),ios::in);
            if (in8) {
                readString2=tempFile3;
            }
            in8.close();
            in9.open((tempFile4),ios::in);
            if (in9) {
                readString2=tempFile4;
            }
            in9.close();
            in10.open((tempFile5),ios::in);
            if (in10) {
                readString2=tempFile5;
            }
            in10.close();
            
            ifstream fin2(readString2.c_str());
            string s2,temp2;
            while( getline(fin2,temp2) )
            {
                s2=s2+temp2;
//                cout<<s2<<endl;
            }
            fin2.close();
            remove(readString2.c_str());
//            cout<<readString2<<" has been removed"<<endl;

            countTempFile--;
//            cout<<s.length()<<" "<<s2.length()<<endl;
            s2=s2+s;
            sort(s2.begin(), s2.end());
            ofstream out(readString2.c_str());
            if (out.is_open()) {
                out<<s2<<endl;
                out.close();
            }

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
    sleep(3);
    

    return 0;
}
