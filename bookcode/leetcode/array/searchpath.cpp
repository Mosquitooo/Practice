
#include<list>
#include<queue>
#include<stack>
#include<string>
#include<iostream>
using namespace std;

char *p[] = 
{
	"hot",
	"dot",
	"dog",
	"lot",
	"log"
};

typedef struct Node
{
	Node*  parent;
	string val;
	Node(Node* par, string key):parent(par),val(key){};
}Node;

bool InDictory(string& key)
{
	for(int i = 0; i < sizeof(p)/sizeof(p[0]); ++i)
	{
		if(!key.compare(p[i])) return true;
	}
	return false;
}

bool IsTravel(list<Node>& TravelList, string& key)
{
	list<Node>::iterator it = TravelList.begin();
	for( ;it != TravelList.end(); ++it)
	{
		if(key.compare(it->val) == 0) return true;
	}
	return false;
}

void PrintPath(Node key)
{
	Node* node = &key;
	while(node)
	{
		cout<< node->val <<"  ";
		node = node->parent;
	}
	cout<<endl;
}

//用双队列来分层
//用指向父节点的链表寻找解
void SearchBFS(string start, string end)
{
	list<Node> str;
	
	str.push_back(Node(NULL, start));
	list<Node>::iterator it = str.begin();
	string front = it->val;
	
	while(it != str.end())
	{
		for(int i = 0; i < front.size(); ++i)
		{
			front = it->val;
			for(char ch = 'a'; ch <= 'z'; ++ch)
			{
				front[i] = ch;
				if(!end.compare(front))
				{
					PrintPath(Node(&(*it), front));
					return; 
				}
				
				if(InDictory(front) && !IsTravel(str, front))
				{
					str.push_back(Node(&(*it), front));
				}
			}
		}
		++it;
	}
}

bool search_next(list<string> path, string& key, const string& end, int index, char c)
{
	string top = key;
	char ch = c;
	for(int i = index; i < top.size(); ++i)
	{
		top = key;
		for(; ch <= 'z'; ++ch)
		{
			top[i] = ch;
			if(!key.compare(top) || (find(path.begin(), path.end(),top) != path.end()))
			{
				continue;
			}
			
			if(InDictory(top) || !end.compare(top))
			{
				key = top;
				return true;
			}
		}
		ch = 'a';
	}
	return false;
}

void PrintStack(list<string> path)
{
	while(!path.empty())
	{
		cout<<path.front()<<" ";
		path.pop_front();
	}
	cout<<endl;
}

int GetCurIndex(const string& curstr, const string& nextstr)
{
	if(curstr.empty() || nextstr.empty())
	{
		return -1;
	}
	
	for(int i = 0; i < curstr.size(); ++i)
	{
		if(curstr[i] != nextstr[i])
			return i;
	}
}

bool SearchDFS(string start, string end)
{
	list<string> path;
	
	path.push_back(start);
	string next;
	int index = 0;
	char ch = 'a';
	
	while(!path.empty())
	{
		string top = path.back();
		
		//cout<<top<<"  "<<next<<endl;
		index = GetCurIndex(top, next);
		
		if(-1 == index)
		{
			index = 0;
			ch = 'a';
		}
		else
		{
			ch = next[index] + 1;
		}
		
		//cout<<index<<" "<<ch<<endl;
		if(search_next(path, top, end, index, ch))
		{
			//cout<<"true "<<top<< endl;
			if(!end.compare(top))
			{
				PrintStack(path);
				next = path.back();
				path.pop_back();
				//return true;
			}
			else
			{
				next.clear();
				path.push_back(top);
			}
		}
		else
		{
			//cout<<"false  "<<top<< endl;
			next = path.back();
			path.pop_back();
		}

	}
}

int main()
{
	//找最优解
	SearchBFS("hit","cog");
	
	//找所有解
	SearchDFS("hit","cog");
	return 0;
}
