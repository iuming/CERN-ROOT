/// \file
/// \ingroup tutorial_roofit
/// \notebook -js
/// Basic functionality: importing data from ROOT TTrees and THx histograms.
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \date July 2008
/// \author Wouter Verkerke

#include "RooRealVar.h"          // Roofit变量类
#include "RooDataSet.h"          // Roofit数据集类
#include "RooDataHist.h"         // Roofit导入直方图类
#include "RooGaussian.h"         // Roofit标准高斯概率密度函数
#include "TCanvas.h"             // 画板类
#include "RooPlot.h"             // Roofit画板类
#include "TTree.h"               // 树类
#include "TH1D.h"                // 直方图类
#include "TRandom.h"             // 随机数类
using namespace RooFit;          // Roofit预命名空间

TH1 *makeTH1();                  // 产生填充了100个mean为0，sigma为3数据的直方图
TTree *makeTTree();              // 产生带有两个branch的树，其中一个为px，填充（0，3）的高斯分布，另一个为py，填充（-15，15）的均匀分布。均填充100个数据

void rf102_dataimport()
{
   // ---------------------------------------------------
   // I m p o r t i n g   R O O T   h i s t o g r a m s
   // ===================================================

   // I m p o r t   T H 1   i n t o   a   R o o D a t a H i s t
   // ---------------------------------------------------------

   // Create a ROOT TH1 histogram
   TH1 *hh = makeTH1();                                                             // 创建直方图hh

   // Declare observable x
   RooRealVar x("x", "x", -10, 10);                                                 // 定义x参数

   // Create a binned dataset that imports contents of TH1 and associates its contents to observable 'x'
   RooDataHist dh("dh", "dh", x, Import(*hh));                                      // 将hh直方图数据转换为Roofit直方图数据集dh

   // P l o t   a n d   f i t   a   R o o D a t a H i s t
   // ---------------------------------------------------

   // Make plot of binned dataset showing Poisson error bars (RooFit default)
   RooPlot *frame = x.frame(Title("Imported TH1 with Poisson error bars"));         // 创建一个Roofit画板
   dh.plotOn(frame);                                                                // 将dh绘制在frame上

   // Fit a Gaussian pdf to the data
   RooRealVar mean("mean", "mean", 0, -10, 10);                                     // 设置mean参数
   RooRealVar sigma("sigma", "sigma", 3, 0.1, 10);                                  // 设置sigma参数
   RooGaussian gauss("gauss", "gauss", x, mean, sigma);                             // 构建概率密度函数gauss
   gauss.fitTo(dh);                                                                 // 使用gauss函数拟合dh数据
   gauss.plotOn(frame);                                                             // 将gauss绘制在frame上

   // P l o t   a n d   f i t   a   R o o D a t a H i s t   w i t h   i n t e r n a l   e r r o r s
   // ---------------------------------------------------------------------------------------------

   // If histogram has custom error (i.e. its contents is does not originate from a Poisson process
   // but e.g. is a sum of weighted events) you can data with symmetric 'sum-of-weights' error instead
   // (same error bars as shown by ROOT)
   RooPlot *frame2 = x.frame(Title("Imported TH1 with internal errors"));           // 创建第二个Roofit画板
   dh.plotOn(frame2, DataError(RooAbsData::SumW2));                                 // 将dh数据绘制在frame2上，其中数据自定义误差不采用默认泊松过程，而是加权事件的总和 见https://root-forum.cern.ch/t/ploton-of-a-roodatahist-with-different-kinds-of-errors/14707
   gauss.plotOn(frame2);                                                            // 将gauss函数绘制在frame2上

   // Please note that error bars shown (Poisson or SumW2) are for visualization only, the are NOT used
   // in a maximum likelihood fit
   //
   // A (binned) ML fit will ALWAYS assume the Poisson error interpretation of data (the mathematical definition
   // of likelihood does not take any external definition of errors). Data with non-unit weights can only be correctly
   // fitted with a chi^2 fit (see rf602_chi2fit.C)

   // -----------------------------------------
   // I m p o r t i n g   R O O T  T T r e e s
   // =========================================

   // I m p o r t   T T r e e   i n t o   a   R o o D a t a S e t
   // -----------------------------------------------------------

   TTree *tree = makeTTree();                                                       // 创建tree

   // Define 2nd observable y 
   RooRealVar y("y", "y", -10, 10);                                                 // 定义变量y

   // Construct unbinned dataset importing tree branches x and y matching between branches and RooRealVars
   // is done by name of the branch/RRV
   //
   // Note that ONLY entries for which x,y have values within their allowed ranges as defined in
   // RooRealVar x and y are imported. Since the y values in the import tree are in the range [-15,15]
   // and RRV y defines a range [-10,10] this means that the RooDataSet below will have less entries than the TTree
   // 'tree'

   RooDataSet ds("ds", "ds", RooArgSet(x, y), Import(*tree));                       // 将tree中数据导入数据集ds

   // U s e   a s c i i   i m p o r t / e x p o r t   f o r   d a t a s e t s
   // ------------------------------------------------------------------------------------
   {
      // Write data to output stream
      std::ofstream outstream("rf102_testData.txt");                                // 创建输出文件流
      // Optionally, adjust the stream here (e.g. std::setprecision) 
      ds.write(outstream);                                                          // 将ds文件输出 为啥只输出64组数据？不是100个吗?
      outstream.close();                                                            // 关闭rf102_testData.txt文件
   }

   // Read data from input stream. The variables of the dataset need to be supplied
   // to the RooDataSet::read() function.
   std::cout << "\n-----------------------\nReading data from ASCII\n";
   RooDataSet *dataReadBack =
      RooDataSet::read("rf102_testData.txt",
                       RooArgList(x, y), // variables to be read. If the file has more fields, these are ignored.
                       "D"); // Prints if a RooFit message stream listens for debug messages. Use Q for quiet. 如果 RooFit 消息流侦听调试消息，则打印。 使用 Q 表示安静。

   dataReadBack->Print("V");

   std::cout << "\nOriginal data, line 20:\n";
   ds.get(20)->Print("V");

   std::cout << "\nRead-back data, line 20:\n";
   dataReadBack->get(20)->Print("V");

   // P l o t   d a t a s e t s  w i t h   m u l t i p l e   b i n n i n g   c h o i c e s
   // ------------------------------------------------------------------------------------

   // Print number of events in dataset
   ds.Print();

   // Print unbinned dataset with default frame binning (100 bins)
   RooPlot *frame3 = y.frame(Title("Unbinned data shown in default frame binning"));
   ds.plotOn(frame3);

   // Print unbinned dataset with custom binning choice (20 bins)
   RooPlot *frame4 = y.frame(Title("Unbinned data shown with custom binning"));
   ds.plotOn(frame4, Binning(20));                                                  // 自定义分bin

   RooPlot *frame5 = y.frame(Title("Unbinned data read back from ASCII file"));
   ds.plotOn(frame5, Binning(20));
   dataReadBack->plotOn(frame5, Binning(20), MarkerColor(kRed), MarkerStyle(5));

   // Draw all frames on a canvas
   TCanvas *c = new TCanvas("rf102_dataimport", "rf102_dataimport", 1000, 800);
   c->Divide(3, 2);
   c->cd(1);
   gPad->SetLeftMargin(0.15);
   frame->GetYaxis()->SetTitleOffset(1.4);
   frame->Draw();
   c->cd(2);
   gPad->SetLeftMargin(0.15);
   frame2->GetYaxis()->SetTitleOffset(1.4);
   frame2->Draw();

   c->cd(4);
   gPad->SetLeftMargin(0.15);
   frame3->GetYaxis()->SetTitleOffset(1.4);
   frame3->Draw();
   c->cd(5);
   gPad->SetLeftMargin(0.15);
   frame4->GetYaxis()->SetTitleOffset(1.4);
   frame4->Draw();
   c->cd(6);
   gPad->SetLeftMargin(0.15);
   frame4->GetYaxis()->SetTitleOffset(1.4);
   frame5->Draw();
}

// Create ROOT TH1 filled with a Gaussian distribution
TH1 *makeTH1()
{
   TH1D *hh = new TH1D("hh", "hh", 25, -10, 10);
   for (int i = 0; i < 100; i++) {
      hh->Fill(gRandom->Gaus(0, 3));
   }
   return hh;
}

// Create ROOT TTree filled with a Gaussian distribution in x and a uniform distribution in y
TTree *makeTTree()
{
   TTree *tree = new TTree("tree", "tree");
   Double_t *px = new Double_t;
   Double_t *py = new Double_t;
   tree->Branch("x", px, "x/D");
   tree->Branch("y", py, "y/D");
   for (int i = 0; i < 100; i++) {
      *px = gRandom->Gaus(0, 3);
      *py = gRandom->Uniform() * 30 - 15;
      tree->Fill();
   }
   return tree;
}
