#include <iostream>
#include <string>
 
using namespace std;
 
 
/*��c++��struct�������������struct�����з��������г�Ա��public��*/
struct Movie/*����ָ��������Ҳ���Բ�ָ��*/
{
    //��Ա����public��
    int ID;
    string Name;
} movie; //����������struct��ʱ������һ��structʵ���������ɶ��˼�أ�
 
 
 
int main(){
    //movie������Movie�ṹ������������
    movie.ID = 100;
    movie.Name = "Face Off";
 
    cout<<"movie.ID = "<<movie.ID<<endl;
    cout<<"movie.Name = "<<movie.Name<<endl;
 
    //����һ������m������Ϊ����ֵ�Ϳ���ʹ������
    Movie m;
    m.ID = 101;
    m.Name = "����˫��";
    cout<<"m.ID="<<m.ID<<endl;
    cout<<"m.Name="<<m.Name<<endl;
 
    //�����ṹ��ָ��
    Movie * mp;
    mp = &m;
    //��ָ���е��ó�ԱʱҪ��->���ţ�mp->ID�ȼ���(*mp).ID
    cout<<"mp->ID = "<<mp->ID<<endl;
    cout<<"mp->Name = "<<mp->Name<<endl;

 	//
 	cout<<"test"<<endl;
 	Movie *tp;
	tp->ID = 111;
	//tp->Name.assign("xx");
	
}

