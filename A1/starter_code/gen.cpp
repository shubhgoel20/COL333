#include <bits/stdc++.h>
using namespace std;

int maxVal=1000000000;

void generateRandomArray(int* array,int l)
{
    for(int x=0;x<l;x++)
    {
        int temp_num=(rand()%maxVal);
        array[x]=temp_num+1;
        // cout<<temp_num<<" "<<maxVal<<"\n";
    }
}

void writeToFile(int* array,int n, ofstream& file)
{
    for(int x=0;x<n;x++)
    {
        file<<array[x]<<" ";
    }
    file<<'\n';
}

int main()
{
    srand(time(0));
    int z,l,time;
    cout<<"enter z:";
    cin>>z;
    cout<<"enter l:";
    cin>>l;
    cout<<"enter time:";
    cin>>time;
    cout<<"enter output file name:";
    string outputfilename;
    cin>>outputfilename;

    ofstream outputFile(outputfilename);
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    outputFile<<time<<'\n';
    outputFile<<z<<'\n';
    outputFile<<l<<'\n';

    int array1[z];
    for(int i=0;i<z;i++)
    {
        generateRandomArray(array1,z);
        writeToFile(array1,z,outputFile);
    }
    vector<int> array2(l);
    vector<vector<int>> matrix(l);
    for(int i=0;i<l;i++)
    {
        generateRandomArray(&array2[0],l);
        matrix[i]=array2;
    }

    for(int i=0;i<l;i++)
    {
        matrix[i][i]=0;
        for(int j=0;j<i;j++)
        {
            matrix[j][i]=matrix[i][j];
        }
    }

    for(int i=0;i<l;i++)
    {
        writeToFile(&matrix[i][0],l,outputFile);
    }
    outputFile.close();
}