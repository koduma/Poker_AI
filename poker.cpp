#include <vector>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <random>
#include <queue>
#include <deque>
#include <list>
#include <map>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <unordered_map>

using namespace std;

int rnd(int mini, int maxi) {
	static mt19937 mt((int)time(0));
	uniform_int_distribution<int> dice(mini, maxi);
	return dice(mt);
}

double d_rnd() {
	static mt19937 mt((int)time(0));
	uniform_real_distribution<double> dice(0, 1.0);
	return dice(mt)*100.0;
}

double pro[9]={17.4,61.2,84.7,89.5,94.1,97.1,99.7,99.87,99.901};
int pattern[9]={23294460,58627800,31433400,6461620,6180020,4047644,3473184,224848,41584};
string type[9]={"HIGH_CARD","ONE_PAIR","TWO_PAIR","THREE_CARD","STRAIGHT","FLUSH","FULL_HOUSE","FOUR_CARD","STRAIGHT_FLUSH"};

double sum_bet[9]={53177,268171,256026,53878,37735,16679,26441,3000,5000};
double looked[9]={153,332,189,40,25,18,17,1,1};

int main(){
    
    
int win=0;
int lose=0;

for(int i=0;i=10000;i++){

int me=20000;
int cpu=20000;
int sb=100;
int bb=200;
int D;

cout<<"sum_bet=";
for(int k=0;k<9;k++){cout<<sum_bet[k]<<",";}
cout<<endl;
    
cout<<"looked=";
for(int k=0;k<9;k++){cout<<looked[k]<<",";}
cout<<endl;

cout<<"win="<<win<<",lose="<<lose<<endl;
    
for(int ddd=0;;ddd++){
    
D=ddd%2;

if(me<=0){   
cout<<"you lose"<<endl;
break;
}
if(cpu<=0){
cout<<"you win"<<endl;
break;
}
//deal_round 
    
double deal1=d_rnd();
double deal2=d_rnd();
int mycard=0;
int cpucard=0;
int myrank=0;
int cpurank=0;

for(int j=0;j<8;j++){
if(pro[j]<deal1&&deal1<=pro[j+1]){
mycard=j+1;
break;
}
}

for(int j=0;j<8;j++){
if(pro[j]<deal2&&deal2<=pro[j+1]){
cpucard=j+1;
break;
}
}

myrank=rnd(0,pattern[mycard]);
cpurank=rnd(0,pattern[cpucard]);    
    
looked[mycard]+=1.0;
    
//bet_round
    
int pot=sb+bb;
int bet=0;
    
bool me_allin=false;
bool cpu_allin=false;
bool breek=false;
    
for(int t=0;;t++){
    
    if(breek){break;}   
    
        if(t==0&&D==0){
        me-=sb;
        cpu-=bb;
        }
        else if(t==0&&D==1){
        me-=bb;
        cpu-=sb;
        }
    
        if(!me_allin&&!cpu_allin){
    
        if(me<=0||cpu<=0){
        if(mycard>cpucard){
            me+=pot;
        }
        else if(mycard==cpucard){
        if(myrank>cpurank){me+=pot;}
        else if(myrank==cpurank){me+=pot/2;cpu+=pot/2;}
        else{cpu+=pot;}
        }
        else{
        cpu+=pot;
        }
        pot=0;
        bet=0;
        if(me<=0){lose++;breek=true;}
        if(cpu<=0){win++;breek=true;}
        cout<<"my_card=("<<type[mycard]<<","<<myrank<<")"<<endl;
        cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl;
        break;
        }
        }
	
	cout<<"-------------------"<<endl;
	cout<<"mychip="<<me<<endl;
	cout<<"cpuchip="<<cpu<<endl;
	cout<<"my_card=("<<type[mycard]<<","<<myrank<<")"<<endl;
	cout<<"pot="<<pot<<endl;
	cout<<"bet="<<bet<<endl;
	cout<<"-------------------"<<endl;
    
        if(D==0){
        cout<<"me_round:"<<t+1<<endl;
            
        cout<<"action(check/bet/call/fold)?=";    
            
        string ans="";
        cin>>ans;
            
        while(1){
            
        if(bet==0&&ans=="check"){break;}
        if((ans=="bet"&&me>=bet&&cpu_allin==false)||(ans=="call"&&bet>0)||ans=="fold"){break;}
        cin>>ans; 
        }   
        //int pattern[9]={23294460,58627800,31433400,6461620,6180020,4047644,3473184,224848,41584};    
        if(ans=="check"){D++;D%=2;}
        else if(ans=="bet"){
        while(1){
        cout<<"bet_size("<<bet<<"~"<<min(me,cpu+bet)<<")?=";
        int tmp;
        cin>>tmp;
        if(bet<=tmp&&tmp<me&&tmp<=cpu+bet){bet=tmp;break;}
        if(tmp==me){
            if(bet<tmp){bet=tmp;}
            break;
        }
        }
        me-=bet;
        pot+=bet;
        sum_bet[mycard]+=(double)bet;
        if(me==0){me_allin=true;}
        else{me_allin=false;}
        D++;D%=2;
        }
        else if(ans=="call"){
            if(cpu_allin==true){
            
            if(me>=bet){//me(call,10000) vs cpu(allin,2300)
            me-=bet;pot+=bet;bet=bet;
            }
            else{//me(call,2300) vs cpu(allin,10000)
            cpu+=bet-me;//10000-2300
            pot+=me;//2300
            pot-=bet-me;
            bet=me;
            me=0;
            }
            
            }
            else{
            if(me<=bet){//me(call,2300) vs cpu(bet,10000)
                cpu+=bet-me;
                pot+=me;
                pot-=bet-me;
                bet=me;
                me=0;
            }//allin
            else{
            me-=bet;pot+=bet;bet=bet;
            }
            }
            
            sum_bet[mycard]+=(double)bet;
            
            if(mycard>cpucard){
            me+=pot;
            }
            else if(mycard==cpucard){
            if(myrank>cpurank){me+=pot;}
            else if(myrank==cpurank){me+=pot/2;cpu+=pot/2;}
            else{cpu+=pot;}
            }
            else{
            cpu+=pot;
            }
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl;
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}  
            break;
        }
        else if(ans=="fold"){
            D++;
            D%=2;
            cpu+=pot;
            pot=0;
            bet=0;
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
        }
        }//D=0
        else{
        cout<<"cpu_round:"<<t+1<<endl;
            
        cout<<"action=";
            
        double div=1.0;
        for(int h=0;h<9;h++){
        div+=sum_bet[h]/(looked[h]+1.0);
        }
        double ai=(sum_bet[cpucard]/(looked[cpucard]+1.0))/div;
        int cpubet=(int)floor((double)cpu*ai*(0.5+(d_rnd()/100.0))/2.0);
        if(cpucard>=2){
        if(cpucard==2){
            if(rnd(1,100)<=10){
            cpubet=cpu;
            }
        }
        else{
        cpubet=cpu;
        }
        }
        if(cpubet>cpu){
        cpubet=cpu;
        }    
        if(cpubet==cpu&&cpubet>0){
            cout<<"cpu=allin="<<cpubet<<endl;
            if(me_allin==true){
                
            if(bet>cpubet){//me(allin,10000) vs cpu(allin,4000)
            me+=bet-cpubet;//10000-4000
            pot+=cpubet;//4000
            pot-=bet-cpubet;//-(10000-4000)
            cpu-=cpubet;bet=cpubet;
            }
            else if(bet<cpubet){//me(allin,4000) vs cpu(allin,10000)
            cpu-=bet;pot+=bet;bet=bet;
            }
            else{
            pot+=bet;cpu-=bet;bet=bet;
            }
                
            if(mycard>cpucard){
            me+=pot;
            }
            else if(mycard==cpucard){
            if(myrank>cpurank){me+=pot;}
            else if(myrank==cpurank){me+=pot/2;cpu+=pot/2;}
            else{cpu+=pot;}
            }
            else{
            cpu+=pot;
            }
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl; 
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
            }
            else{
                
            if(bet<cpubet){//me(bet,5000) vs cpu(allin,10000)
            pot+=cpubet;cpu-=cpubet;bet=cpubet;
            }
            else{
                
            if(bet>cpubet){//me(bet,5200) vs cpu(allin,2000)
            me+=bet-cpubet;
            pot+=cpubet;
            pot-=bet-cpubet;
            cpu-=cpubet;bet=cpubet;
            }
            else{
            pot+=bet;cpu-=bet;bet=bet;
            }
                
            if(mycard>cpucard){
            me+=pot;
            }
            else if(mycard==cpucard){
            if(myrank>cpurank){me+=pot;}
            else if(myrank==cpurank){me+=pot/2;cpu+=pot/2;}
            else{cpu+=pot;}
            }
            else{
            cpu+=pot;
            }
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl; 
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
            
            }
            if(cpu==0){
            cpu_allin=true;
            }
            else{
            cpu_allin=false;
            }
            D++;D%=2;
            }
        }
        else if(cpubet==0){
            cout<<"cpu=fold"<<endl;
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl;
            me+=pot;
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
        }
        else{
            if(cpubet>bet){
            cout<<"cpu=bet="<<cpubet<<endl;
            pot+=cpubet;cpu-=cpubet;bet=cpubet;
            D++;D%=2;
            }
            else if(cpubet==bet){
            cout<<"cpu=call"<<endl;
            pot+=cpubet;cpu-=cpubet;bet=cpubet;
                
            if(mycard>cpucard){
            me+=pot;
            }
            else if(mycard==cpucard){
            if(myrank>cpurank){me+=pot;}
            else if(myrank==cpurank){me+=pot/2;cpu+=pot/2;}
            else{cpu+=pot;}
            }
            else{
            cpu+=pot;
            }
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl; 
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
                
            }
            else{
            cout<<"cpu=fold"<<endl;
            cout<<"cpu_card=("<<type[cpucard]<<","<<cpurank<<")"<<endl;
            me+=pot;
            D++;D%=2;
            pot=0;
            bet=0;
            if(me<=0){lose++;breek=true;}
            if(cpu<=0){win++;breek=true;}
            break;
            }
        }   
     }
}
}    
}


return 0;
}
