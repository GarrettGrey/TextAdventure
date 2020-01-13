/*
	This program runs the attack algorithm 10,000 times and outputs the results to a
	.csv file. I would then use this file to put the values to a bar graph to see how
	many times certain damage outputs appear.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdlib.h> 

using namespace std;

ofstream file("graph.csv");
vector<int> val;

int att(){
	return(rand() % 10) + 1;
}

int defend(){
	int ret = (rand() % 10) + 1;
	return ret * (rand() % 10) + 1;
}

int crit(){
	int crit = (rand() % 10) + 1;
	if(crit == 1 || crit == 2 || crit == 8)return 2;
	return 1;
}

vector<int> bubbleSort(const vector<int> &orig){
    vector<int> ret;
    for(int i = 0; i < orig.size(); i++){
        ret.push_back(orig[i]);
    }
    for(int j = 1; j < ret.size(); j++){
        if(ret[j-1] > ret[j]){
            int temp = ret[j-1];
            ret[j-1] = ret[j];
            ret[j] = temp;
        }
    }
    for(int k = 1; k < ret.size(); k++){
        if(ret[k-1] > ret[k]){
            return bubbleSort(ret);
        }    
    }
    return ret;
}

int main(){
    srand (time(NULL));
    for(int i = 0; i < 10000; i++){
        int c = crit();
		int a = att();
		int d = defend();
		a *= d;
		if (d < 25){
			a /= 5;
		}else if (d < 50){
			a /= 15;
		}else a /= d;
		if (a == 0 || a == 1 || a == 2){
			a = att();
			d = defend();
			a *= d;
			if (d < 25){
				a /= 25;
			}else if (d < 50){
				a /= 35;
			}else a /= 60;
			if (a == 0){
				a += rand() % 3;
			}
		}
		if (a <= 10){
			if(a * c > 10) a*=c;
			val.push_back(a);
		}else i--;
    }
    val = bubbleSort(val);
	/*for (int i = 0; i < val.size(); i++){
		file << i << "," << val[i] << endl;
	}*/
	int count = 0;
	for (int j = 0; j <= 20; j++){	
		for (int i = 0; i < val.size(); i++){
			if(val[i] == j)count++;
		}
		file << j << "," << count << endl;
		count = 0;
	}
    file.close();
}
