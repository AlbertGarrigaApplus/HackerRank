#include <iostream>
#include <sstream>
//#include "Database.h"

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
string printQueryResult(vector<Tag> lst, string text){
    string tagVal;
     char charChoice;
    //sscanf(text.c_str(),"%s",&tagVal);
    if(text.find('.')!=std::string::npos){
        tagVal=split(text,'.')[0];
        charChoice='.';
        }
    else{
        if(text.find('~')!=std::string::npos){
            tagVal=split(text,'~')[0];  
            charChoice='~';
        }
        
    }
    if(charChoice=='.'){
        for(int i = 0 ; i<lst.size();i++){
            if(lst[i].checkTag(tagVal)){
                int find2 = text.find('.');
                std::string token = text.substr(find2+1,text.length());
                string val = printQueryResult(lst[i].getSons(),token);
                if(val!="") return val;
            }
        }

    }
     if(charChoice=='~'){
        for(int i = 0 ; i<lst.size();i++){
              int find2 = text.find('~');
              std::string token = text.substr(find2+1,text.length());
                if(lst[i].checkTag(tagVal)){
                if(!lst[i].hasAttr(token)) return  "Not Found!";
                else{
                    string val = lst[i].getVal(token);
                    return val;
                }
              }
        }
     }
     return "";
}
string getLineFunc(){
    string tagString;
    getline(cin,tagString);
    if(tagString ==""){
        tagString=getLineFunc();                    
    }
    return tagString;
}
string getNameFromTag(string from){
     string spplited = split(from,' ')[0];
     string tagName = split(spplited,'<')[1];
     if(tagName[tagName.length()-1]=='>')return split(tagName,'>')[0];
     return tagName;
}
class Tag{
    string tagName;
    map<string,string> mapvalue; 
    vector<Tag> sons;    
    Tag *pre=nullptr;
    bool isOpen=true;
    public:
        int level=0;
        Tag(){}
        Tag(string t,Tag *d){
            tagName=t;           
            if(d)pre = d;
            if(!pre){
                level = 1;
            }
            else{
                level = pre->level+1;
            }
        }
        bool hasAttr(string token){
            std::map<string,string>::iterator it = mapvalue.begin();
            while(it!=mapvalue.end()){
                if(it->first==token)return true;
                it++;
            }
            return false;
        } 
        string getVal(string token){
            std::map<string,string>::iterator it = mapvalue.begin();
            while(it!=mapvalue.end()){
                if(it->first==token)return it->second;
                it++;
            }
            return "Not Found!";
        }
        string getAttrName(string attrname){
            string s = mapvalue.find(attrname)->second;
            return s;
        }
        void addAttrVal(string attr, string val){
            mapvalue.insert(std::pair<string,string>(attr,val));
        }
        //void printVal(){cout << value <<endl;}
        vector<Tag> getSons(){return sons;}
        bool checkTag(string tagName1){ 
            return tagName1==tagName;
        }
        Tag *getDad(){
            return pre;
        }
        void printElement(){
            for(int i =0;i<level;i++){
                cout << "|-";
            }
           // cout << "Tag:"<<tagName<<" Attr:"<<attrName<<" ='"<<value<<"' SonsSize:" <<sons.size()<<endl;
            for(int i = 0; i<sons.size();i++){
                sons[i].printElement();
            }
        }
        
        void setDad(Tag *d){
            pre = d;
        }
        Tag* createSon(Tag e){
            
            //cout<<tagName<<" Adding son";
            sons.push_back(e);
            return &sons[sons.size()-1];
        }
};

int main() {
    /*Reproduce HackerRank Inputs*/
    string text1 ="4 3\n<tag1 value = \"HelloWorld\">\n<tag2 name = \"Name1\">\n</tag2>\n</tag1>\ntag1.tag2~name\ntag1~name\ntag1~value";
    string text2="16 14\n<tag1 v1 = \"123\" v2 = \"43.4\" v3 = \"hello\">\n</tag1>\n<tag2 v4= \"v2\" name = \"Tag2\">\n<tag3 v1 = \"Hello\" v2 = \"World!\">\n</tag3>\n<tag4 v1 = \"Hello\" v2 = \"Universe!\">\n</tag4>\n</tag2>\n<tag5>\n<tag7 new_val = \"New\">\n</tag7>\n</tag5>\n<tag6>\n<tag8 intval = \"34\" floatval = \"9.845\">\n</tag8>\n</tag6>\ntag1~v1\ntag1~v2\ntag1~v3\ntag4~v2\ntag2.tag4~v1\ntag2.tag4~v2\ntag2.tag3~v2\ntag5.tag7~new_val\ntag5~new_val\ntag7~new_val\ntag6.tag8~intval\ntag6.tag8~floatval\ntag6.tag8~val\ntag8~intval";
    istringstream iss(text2);
    cin.rdbuf(iss.rdbuf());
    //End -Reproduce HackerRank Inputs*/
    

    int nT, nQ;
    cin >> nT >> nQ;
    vector<Tag> lstVector;
    vector<string> queries;
    Tag *actualTag = nullptr;
    
    for(int i = 0; i<nT;i++){
        string tagString=""; 
        getline(cin,tagString);
        if(tagString ==""){
            getline(cin,tagString);
        }  
        string val1;
        if((tagString[0]=='<') && (tagString[1]!='/')){     
            int numAttr = 0;
            for(int i =0; i<tagString.length();i++){
                if(tagString[i]=='='){numAttr +=1;}
            }
            ///cout << "N Occurrences = "<<numAttr << tagString <<endl;
            string tagName = getNameFromTag(tagString);
            Tag xa = Tag(tagName,actualTag);
            vector<string> attrs= split(tagString,' ');
            for(int i=0;i<attrs.size();i++){
                if(attrs[i]=="="){
                    xa.addAttrVal(attrs[i-1],split(attrs[i+1],'\"')[1]);
                }
            }
            if(!actualTag){
                lstVector.push_back(xa);
                actualTag = &lstVector[lstVector.size()-1];
            }
            else{
               // Tag xa = Tag(n,actualTag);
                actualTag =actualTag->createSon(xa);
            }
        }
        else if((tagString[0]=='<') && (tagString[1]=='/')){
           
            if(actualTag->getDad()){
               actualTag = actualTag->getDad();
            } 
            else{
                actualTag=nullptr;
            }
        }
        
        
    }
    //Apply Queries
    for(int i = 0; i<nQ;i++){
        string tagString=getLineFunc();
        string val=printQueryResult(lstVector,tagString);
        if(val=="")cout << "Not Found!"<<endl;
        else cout << val << endl;
        
    }

    //cout << "Num Queries: "<< nQ <<endl;
    return 0;
}
