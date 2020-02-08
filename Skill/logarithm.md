# 对数输出    
## root中TGraphErrors的拟合和对数坐标轴的设置    
TGraphErrors是带有errorbar的TGraph,拟合和TGraph一样,不过要注意拟合前指定初值和限制,要不然容易出错    
对数坐标轴的限制是通过canvas中的SetLogx来实现的,如新生成一个canvas后                 
``` 
c1 = new TCanvas("c1","",200,10,700,500);       
c1->SetLogx(); 
``` 
这样就可以使canvas中的x轴变成对数形式       
来自[希格思的等待](http://blog.sina.com.cn/u/1354171670)           
## 对数坐标设置函数
首先先建立一块画板：`TCanvas *c1 = new TCanvas("c1","transparentpad",200,10,700,500);`            
设置画板上的图形：`TPad *pad1 = newTPad("pad1","",0,0,1,1);`       
设置x以对数形式输出：`pad1->SetLogx(); `     
设置y以对数形式输出：`pad1->SetLogy(); `       
