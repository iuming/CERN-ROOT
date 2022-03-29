/// \file
/// \ingroup tutorial_roofit
/// \notebook -js
/// Basic functionality: fitting, plotting, toy data generation on one-dimensional PDFs.
///
///  pdf = gauss(x,m,s)
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \date July 2008
/// \author Wouter Verkerke

#include "RooRealVar.h"       // Roofit变量
#include "RooDataSet.h"       // Roofit数据集
#include "RooGaussian.h"      // Roofit标准概率密度高斯函数
#include "TCanvas.h"          // 画板类
#include "RooPlot.h"          // Roofit绘图类
#include "TAxis.h"            // 坐标轴类
using namespace RooFit;       // Roofit预命名空间

void rf101_basics()
{
   // S e t u p   m o d e l
   // ---------------------

   // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
   RooRealVar x("x", "x", -10, 10);                                     // 定义x变量，最小值为-10，最大值为10
   RooRealVar mean("mean", "mean of gaussian", 1, -10, 10);             // 定义mean变量，初始值定义为1，范围为（-10，10）
   RooRealVar sigma("sigma", "width of gaussian", 1, 0.1, 10);          // 定义sigma变量，初始值定义为1，范围为（0.1，10）

   // Build gaussian pdf in terms of x,mean and sigma
   RooGaussian gauss("gauss", "gaussian PDF", x, mean, sigma);          // 构建高斯概率密度函数gauss

   // Construct plot frame in 'x'
   RooPlot *xframe = x.frame(Title("Gaussian pdf."));                   // 构建xframe画板，设置标题

   // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
   // ---------------------------------------------------------------------------

   // Plot gauss in frame (i.e. in x)
   gauss.plotOn(xframe);                                                // 将gauss函数绘制在xframe画板上

   // Change the value of sigma to 3
   sigma.setVal(3);                                                     // 将sigma参数改为3

   // Plot gauss in frame (i.e. in x) and draw frame on canvas
   gauss.plotOn(xframe, LineColor(kRed));                               // 将gauss函数绘制在xframe上，颜色改为红色

   // G e n e r a t e   e v e n t s
   // -----------------------------

   // Generate a dataset of 1000 events in x from gauss
   RooDataSet *data = gauss.generate(x, 10000);                         // 定义data数据集，使用gauss概论密度函数产生10000个数据进行填充

   // Make a second plot frame in x and draw both the
   // data and the pdf in the frame
   RooPlot *xframe2 = x.frame(Title("Gaussian pdf with data"));         // 构建xframe2画板
   data->plotOn(xframe2);                                               // 将data数据集绘制在xframe2上
   gauss.plotOn(xframe2);                                               // 将gauss函数绘制在xframe2上

   // F i t   m o d e l   t o   d a t a
   // -----------------------------

   // Fit pdf to data
   gauss.fitTo(*data);                                                  // 使用gauss函数拟合data数据

   // Print values of mean and sigma (that now reflect fitted values and errors)
   mean.Print();                                                        // 将拟合参数mean打印至屏幕
   sigma.Print();                                                       // 将拟合参数sigma打印至屏幕

   // Draw all frames on a canvas
   TCanvas *c = new TCanvas("rf101_basics", "rf101_basics", 800, 400);  // 构建画板
   c->Divide(2);                                                        // 将画板分为两个区域
   c->cd(1);                                                            // 定位到第一块区域
   gPad->SetLeftMargin(0.15);                                           // 设置左边边距为0.15
   xframe->GetYaxis()->SetTitleOffset(1.6);                             // 设置xframe的Y轴的标题大小为1.6
   xframe->Draw();                                                      // 将xframe绘制在该区域
   c->cd(2);                                                            // 定位到第二块区域
   gPad->SetLeftMargin(0.15);                                           // 设置左边区域为0.15
   xframe2->GetYaxis()->SetTitleOffset(1.6);                            // 设置xframe2的Y轴的标题大小为1.6
   xframe2->Draw();                                                     // 将xframe2绘制在这个区域
}
