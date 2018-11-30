#include <iostream>
#include <string>
 
using namespace std;
 
 
/*在c++中struct和类的区别在于struct不能有方法，所有成员是public的*/
struct Movie/*可以指定类型名也可以不指定*/
{
    //成员都是public的
    int ID;
    string Name;
} movie; //可以在声明struct的时候声明一个struct实例，这个有啥意思呢？
 
 
 
int main(){
    //movie变量在Movie结构声明处定义了
    movie.ID = 100;
    movie.Name = "Face Off";
 
    cout<<"movie.ID = "<<movie.ID<<endl;
    cout<<"movie.Name = "<<movie.Name<<endl;
 
    //声明一个变量m，无须为赋初值就可以使用它了
    Movie m;
    m.ID = 101;
    m.Name = "多面双雄";
    cout<<"m.ID="<<m.ID<<endl;
    cout<<"m.Name="<<m.Name<<endl;
 
    //声明结构的指针
    Movie * mp;
    mp = &m;
    //在指针中调用成员时要用->符号，mp->ID等价于(*mp).ID
    cout<<"mp->ID = "<<mp->ID<<endl;
    cout<<"mp->Name = "<<mp->Name<<endl;

 	//
 	cout<<"test"<<endl;
 	Movie *tp;
	tp->ID = 111;
	//tp->Name.assign("xx");
	
}

