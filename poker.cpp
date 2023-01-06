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

double pro[9]={17.4,61.2,84.7,89.5,94.1,97.1,99.7,99.87,99.901};
int pattern[9]={23294460,58627800,31433400,6461620,6180020,4047644,3473184,224848,41584};
string type[9]={"HIGH_CARD","ONE_PAIR","TWO_PAIR","THREE_CARD","STRAIGHT","FLUSH","FULL_HOUSE","FOUR_CARD","STRAIGHT_FLUSH"};
double prev_looked[9]={153,332,189,40,25,18,17,1,1};
double sum_bet[9]={53177,268171,256026,53878,37735,16679,26441,3000,5000};
double looked[9];
double bed[9]={53177,268171,256026,53878,37735,16679,26441,3000,5000};
double led[9]={153,332,189,40,25,18,17,1,1};
//double led[9]={62,400,394,31,17,351,110,113,251};

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

struct gamestate{
int mychip;
int enemychip;
int sb;
int bb;
int D;
int pot;
int mybet;
int enemybet;
int mycard;
int cpucard;
double myrank;
double cpurank;
bool myallin;
bool enemyallin;
int fight;
    
void init(){
mychip=20000;
enemychip=20000;
D=0;    
}    

void deal(){
sb=100;
bb=200;
pot=0;    
mybet=0;
enemybet=0;
double deal1=d_rnd();
double deal2=d_rnd();
    
mycard=0;    
for(int j=0;j<8;j++){
if(pro[j]<deal1&&deal1<=pro[j+1]){
mycard=j+1;
break;
}
}

cpucard=0;    
for(int j=0;j<8;j++){
if(pro[j]<deal2&&deal2<=pro[j+1]){
cpucard=j+1;
break;
}
}

myrank=(double)(pattern[mycard]-rnd(1,pattern[mycard]))*100.0/(double)pattern[mycard];
cpurank=(double)(pattern[cpucard]-rnd(1,pattern[cpucard]))*100.0/(double)pattern[cpucard];       
myallin=false;
enemyallin=false;
fight=0;    

if(D==0){
mychip-=sb;
enemychip-=bb;
mybet=sb;
enemybet=bb;    
}
else if(D==1){
mychip-=bb;
enemychip-=sb;
mybet=bb;
enemybet=sb;    
}

pot=sb+bb;    
    
}

bool showdown(){
if(mycard>cpucard){
mychip+=pot;
}
else if(mycard==cpucard){
if(myrank<cpurank){mychip+=pot;}
else if(myrank==cpurank){mychip+=pot/2;enemychip+=pot/2;}
else{enemychip+=pot;}
}
else{
enemychip+=pot;
}
return true;    
}    
    
bool judge(){
if(enemychip<=0){return true;}
if(mychip<=0){return true;}
return false;    
}    
    
bool Ibetting(int bet){
    if(mychip+mybet<bet){return false;}
    if(enemybet>bet){return false;}
    if(mybet>bet){return false;}
    mychip-=bet-mybet;
    pot+=bet-mybet;
    mybet=bet;
    D++;
    D%=2;
    if(mychip==0){
    myallin=true;
    }
    return true;
}
bool enemybetting(int bet){
    if(enemychip+enemybet<bet){return false;}
    if(mybet>bet){return false;}
    if(enemybet>bet){return false;}
    enemychip-=bet-enemybet;
    pot+=bet-enemybet;
    enemybet=bet;
    D++;
    D%=2;
    if(enemychip==0){
    enemyallin=true;
    }
    return true;
}
bool Icheck(){
if(mybet==0&&enemybet==0){
    D++;
    D%=2;
    return true;
}
return false;    
}    
bool enemycheck(){
if(mybet==0&&enemybet==0){
    D++;
    D%=2;
    return true;
}
return false;    
}
    
bool Icall(){
    if(mybet>enemybet){return false;}
    if(mychip+mybet<=enemybet){
    pot+=mychip;    
    mybet+=mychip;
    pot-=enemybet-mybet;
    enemychip+=enemybet-mybet;    
    mychip=0;        
    }
    else{
    mychip-=enemybet-mybet;
    pot+=enemybet-mybet;
    mybet=enemybet;
    }    
    D++;
    D%=2;
    return true;
}    
bool enemycall(){
    if(enemybet>mybet){return false;}
    if(enemychip+enemybet<=mybet){
    pot+=enemychip;
    enemybet+=enemychip;
    pot-=mybet-enemybet;
    mychip+=mybet-enemybet;    
    enemychip=0;
    }
    else{
    enemychip-=mybet-enemybet;
    pot+=mybet-enemybet;
    enemybet=mybet;
    }    
    D++;
    D%=2;
    return true;
}    
bool Ifold(){
D++;
D%=2;    
enemychip+=pot;
pot=0;
mybet=0;
enemybet=0;
return true;
}    
bool enemyfold(){
D++;
D%=2;    
mychip+=pot;
pot=0;
mybet=0;
enemybet=0;
return true;
}     
    
    
};

struct game{    
//double enemy_sum_bet[9]={16000,70000,26000,10000,30374,4000,5000,5000,5000};
//double enemy_looked[9]={8,36,14,5,7,2,1,1,1};
double enemy_sum_bet[9]={0};
double enemy_looked[9]={0};    
    
void battle(){     
    
int win=0;
int lose=0;

gamestate gs;
    
gs.init();    
    
for(int i=0;i=100;i++){    
    
while(1){    
gs.deal();
enemy_looked[gs.mycard]++;
if(gs.mychip<=0){lose++;cout<<"youlose"<<endl;return;}
if(gs.enemychip<=0){win++;cout<<"youwin"<<endl;return;}     
    
while(1){    

cout<<"round:"<<gs.fight+1<<endl;
    
cout<<"-------------------"<<endl;
cout<<"mychip="<<gs.mychip<<endl;
cout<<"cpuchip="<<gs.enemychip<<endl;
cout<<"my_card=("<<type[gs.mycard]<<",top"<<gs.myrank<<"%)"<<endl;
cout<<"pot="<<gs.pot<<endl;
cout<<"bet="<<gs.enemybet<<endl;    
cout<<"-------------------"<<endl;    
    
if(gs.D==0){
cout<<"action(bet/call/fold)?=";    
string ans="";
cin>>ans;
while(1){
/*    
if(ans=="check"){
    if(gs.Icheck()){
    break;
    }    
}
*/
if(ans=="bet"){
if(gs.mychip>=gs.enemybet&&(!gs.enemyallin)){break;}
}
if(ans=="call"){
if(gs.Icall()){break;}
}
if(ans=="fold"){
if(gs.Ifold()){break;}
}
cin>>ans;  
}
if(ans=="bet"){
while(1){
cout<<"bet_size("<<gs.enemybet<<"~"<<min(gs.mychip+gs.mybet,gs.enemychip+gs.enemybet)<<")?=";
int tmp;
cin>>tmp;
if(gs.enemybet<=tmp&&tmp<=min(gs.mychip+gs.mybet,gs.enemychip+gs.enemybet)){
    gs.Ibetting(tmp);
    enemy_sum_bet[gs.mycard]+=(double)tmp;
    if(tmp==gs.enemybet){
    gs.showdown();
    if(gs.mychip<=0){lose++;cout<<"youlose"<<endl;return;}
    if(gs.enemychip<=0){win++;cout<<"youwin"<<endl;return;}    
    }
    break;
} 
}
}
else if(ans=="call"){
enemy_sum_bet[gs.mycard]+=(double)gs.mybet;
cout<<"cpu_card=("<<type[gs.cpucard]<<",top"<<gs.cpurank<<"%)"<<endl;
gs.showdown();
if(gs.mychip<=0){lose++;cout<<"youlose"<<endl;return;}
if(gs.enemychip<=0){win++;cout<<"youwin"<<endl;return;}     
break;    
}
else if(ans=="fold"){
cout<<"cpu_card=("<<type[gs.cpucard]<<",top"<<gs.cpurank<<"%)"<<endl;    
break;
}    
}
else{            
double div=1.0;
for(int h=0;h<9;h++){
div+=bed[h]/(led[h]+1.0);
}
int money=gs.enemychip+gs.enemybet;  
double ai=(bed[gs.cpucard]/(led[gs.cpucard]+1.0))/div;
int cpubet=(int)floor((double)money*ai*(0.5+(d_rnd()/100.0))/2.0);
if(gs.cpucard>=2){
if(gs.cpucard==2){
if(rnd(1,100)<=10){
cpubet=money;
}
}
else{
cpubet=money;
}
}
if(cpubet>money){
cpubet=money;
}
    
int act=0;    
    
bool ret=gs.enemybetting(cpubet);
if(!ret){
ret=gs.enemyfold();
act=1;    
}
    
if(act==0){
cout<<"cpu=action=bet="<<cpubet<<endl;
}
else if(act==1){
cout<<"cpu=action=fold"<<endl;
cout<<"cpu_card=("<<type[gs.cpucard]<<",top"<<gs.cpurank<<"%)"<<endl;   
break;    
}   
    
}

gs.fight++;    
}
}
}
}    
    
int self_battle(){    
    
gamestate gs;
    
gs.init();    
    
for(int i=0;i=100;i++){  
    
while(1){    
gs.deal();
if(gs.mychip<=0){return 0;}
if(gs.enemychip<=0){return 1;}    
    
while(1){    

if(gs.D==0){
double div=1.0;
for(int h=0;h<9;h++){
div+=sum_bet[h]/(looked[h]+1.0);
}
int money=gs.mychip+gs.mybet;    
double ai=(sum_bet[gs.mycard]/(looked[gs.mycard]+1.0))/div;
int mbet=(int)floor((double)money*ai*(0.5+(d_rnd()/100.0))/2.0);
if(gs.mycard>=2){
if(gs.mycard==2){
if(rnd(1,100)<=10){
mbet=money;
}
}
else{
mbet=money;
}
}
if(mbet>money){
mbet=money;
}
    
bool ret=gs.Ibetting(mbet);
if(!ret){
ret=gs.Ifold();
break;    
}
else{
if(mbet==gs.enemybet){
gs.showdown();
if(gs.mychip<=0){return 0;}
if(gs.enemychip<=0){return 1;} 
}
}    
}
else{            
double div=1.0;
for(int h=0;h<9;h++){
div+=enemy_sum_bet[h]/(enemy_looked[h]+1.0);
}
int money=gs.enemychip+gs.enemybet;    
double ai=(enemy_sum_bet[gs.cpucard]/(enemy_looked[gs.cpucard]+1.0))/div;
int cpubet=(int)floor((double)money*ai*(0.5+(d_rnd()/100.0))/2.0);
if(gs.cpucard>=2){
if(gs.cpucard==2){
if(rnd(1,100)<=10){
cpubet=money;
}
}
else{
cpubet=money;
}
}
if(cpubet>money){
cpubet=money;
}
    
bool ret=gs.enemybetting(cpubet);
if(!ret){
ret=gs.enemyfold();
break;    
}
else{
if(cpubet==gs.mybet){    
gs.showdown();
if(gs.mychip<=0){return 0;}
if(gs.enemychip<=0){return 1;}    
}    
}    
    
}

gs.fight++;    
}
}
} 

return 0;    
}   
    
};


int main(){
    
    game g;
    
    g.battle();
    
    cout<<"sum_bet=";
    for(int k=0;k<9;k++){cout<<g.enemy_sum_bet[k]<<",";}
    cout<<endl;
    
    cout<<"looked=";
    for(int k=0;k<9;k++){cout<<g.enemy_looked[k]<<",";}
    cout<<endl;
    
    for(int i=0;i<100000;i++){    
    int win=0;
    if((i+1)%1000==0){    
    printf("train=%d/%d\n",i+1,100000);
    }
    memcpy(looked,prev_looked,sizeof(looked));
    for(int j=0;j<9;j++){
    int low=0;
    if(looked[j]<0.1){low=0;}
    else{low=-1;}
    looked[j]+=(double)rnd(low,1);
    }    
    for(int j=0;j<100;j++){    
    win+=g.self_battle();
    }
    if(win>50){    
    memcpy(prev_looked,looked,sizeof(looked));
    }    
    }
    
    cout<<"looked=";
    for(int k=0;k<9;k++){cout<<prev_looked[k]<<",";}
    cout<<endl;
    
    memcpy(led,prev_looked,sizeof(led));
    
    g.battle();


    return 0;
};
