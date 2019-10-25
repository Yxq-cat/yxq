#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
const int WAY=4;
const int RIGHT= 0;

const int DOWN= 1;
const int LEFT= 2;
const int UP= 3;
const int YES=5;
const int NO=6;



class Node{
  public:
    Node(){
    for(int i=0;i<WAY;++i){
      arr[i]=NO;
      }
    }
    void setVal(int data){
      val=data;
    }
    void  setX(int x){
      mx=x;
    }
    void setY(int y){
      my=y;
    }
    void setarr(int state,int way){
      arr[way]=state;
    }
    int getval(){
      return val;
    }
    int getmx(){
      return mx;
    }
    int getmy(){
      return my;
    }
    int getarr(int way){
      return arr[way];
    }

  private:
    int val;//存结点的值
    int mx;//x坐标
    int my;//y坐标
    int arr[WAY];//行走状态

};

class SeqStack{
  public:
    SeqStack(int size=10):
      mtop(0),
      msize(size)
    {
    PS=new Node[msize];

    }
    ~SeqStack(){
      delete[]PS;
      PS=NULL;
    }

    SeqStack(const SeqStack &)=delete;
    SeqStack & operator=(const SeqStack &)=delete;
    
    void push(const Node &node){
    
      if(full()){
        expand();
      }
      PS[mtop++]=node;
    }

    Node pop(){
      if(empty()){
        throw "stack is empty,pop fail";//抛出异常
      }
      --mtop;
      return PS[mtop];
    }

    Node top(){
      return PS[mtop-1];
    }

    bool empty(){
      return mtop==0;
    }
    bool full(){
      return mtop==msize;
    }
  private:
    Node *PS;
    int mtop;
    int msize;
    void expand(){
      Node *Ps=new Node[msize*2];
      for(int i=0;i<msize;++i){
        Ps[i]=PS[i];
      }
      delete[]PS;
      PS=Ps;
      msize*=2;
    }
};

class Maze{

  public:
    Maze(int row,int col)
      :mRow(row)
      ,mCol(col)
    {
      mpCell=new Node*[mRow];
      for(int i=0;i<mRow;i++){
        mpCell[i]=new Node[mCol];
      }
    }

    void initNode(int data,int x,int y){
      mpCell[x][y].setVal(data);
      mpCell[x][y].setX(x);
      mpCell[x][y].setY(y);
    }

//调整迷宫路径节点的方向的行走状态
    void setMazePath(){
      for(int i=0;i<mRow;++i){
        for(int j=0;j<mCol;++j){
          if(mpCell[i][j].getval()==1){
            continue;
          }
          //调整右方向的状态
          if(j<mCol-1&&mpCell[i][j+1].getval()==0){
            mpCell[i][j].setarr(RIGHT,YES);
          }
          //调整下方向的状态
          if(i<mRow-1&& mpCell[i+1][j].getval()==0){
            mpCell[i][j].setarr(DOWN,YES);
          }
          //调整左方向的状态
          if(j>0 && mpCell[i][j-1].getval()==0){
            mpCell[i][j].setarr(LEFT,YES);
          }
          //调整上方向的状态
          if(i>0 && mpCell[i-1][j].getval()==0){
            mpCell[i][j].setarr(UP,YES);
          }

        }
      }

    }
    void findMazePath(){
      //没有入口
      if(mpCell[0][0].getval()==1){
        return ;
      }
      //没有出口
      if(mpCell[mRow-1][mCol-1].getval()==1){
        return ;
      }
      //把入口节点入栈
      
      mStack.push(mpCell[0][0]);
      
      while(!mStack.empty()){
      
        Node top=mStack.top();

        int x=top.getmx();
        int y=top.getmy();
        //迷宫出口已经找到
        if(x==mRow-1 && y==mCol-1){
          return ;
        }

        //判断节点右方向
        if(mpCell[x][y].getarr(RIGHT)==YES){
          mpCell[x][y].setarr(RIGHT,NO);
          mpCell[x][y+1].setarr(LEFT,NO);
          //把当前结点的下一个结点入栈
          mStack.push(mpCell[x][y+1]);
          continue;
        }

        //判断结点的下方向
        if(mpCell[x][y].getarr(DOWN)==YES){
          mpCell[x][y].setarr(DOWN,NO);
          mpCell[x+1][y].setarr(UP,NO);
          mStack.push(mpCell[x+1][y]);
          continue;
        }

        //判断左
      
        if(mpCell[x][y].getarr(LEFT)==YES){
          mpCell[x][y].setarr(LEFT,NO);
          mpCell[x][y-1].setarr(RIGHT,NO);
          mStack.push(mpCell[x][y-1]);
          continue;
        }

        //判断上
        
        if(mpCell[x][y].getarr(UP)==YES){
          mpCell[x][y].setarr(UP,NO);
          mpCell[x-1][y].setarr(DOWN,NO);
          mStack.push(mpCell[x-1][y]);
          continue;
        }
        //都不能走就出栈
        mStack.pop();
      }

}
    void showMazePath(){
      if(mStack.empty()){
        cout<<"没有迷宫路径"<<endl;
      }
      else{
        cout<<"路径如下"<<endl;
        while(!mStack.empty()){
          Node node=mStack.pop();
          mpCell[node.getmx()][node.getmy()].setVal('*');

        }
        for(int i=0;i<mRow;i++){
          for(int j=0;j<mCol;j++){
            if(mpCell[i][j].getval()=='*'){
              cout<<'*'<<" ";
            }
            else{
              cout<<mpCell[i][j].getval()<<" ";
            }
          }
        cout<<endl;
        }
      }
    }
  private:
    int mRow;
    int mCol;
    Node **mpCell;//存储迷宫路经的二维数组
    SeqStack mStack;


};

int main(){
  cout<<"请输入迷宫行列数";
  int row,col;
  cin>>row>>col;
  Maze maze(row,col);
  cout<<"输入迷宫路径信息"<<endl;
  int data=0;
  for(int i=0;i<row;++i){
    for(int j=0;j<col;++j){
      cin>>data;
      maze.initNode(data,i,j);
    }
  }
  //调整迷宫路径中的每一个节点的行走状态
  maze.setMazePath();
  maze.findMazePath();

  maze.showMazePath();
}
