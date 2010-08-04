using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using dshow;
using dshow.Core;
using System.Threading;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

/*
 *  CamTrace, version 1;
 *  this code uses the Dshow.dll wich is a part of directx windows api
 *  each new frame from capture device is delivered to image processing function 
 *  by an event discribed in Events.cs and generated in CaptureDevice.cs
 *  Each new frame generates absolute coordinates of exX and exY
 *  
 *                           Author: Dmitry Paradny.
 */

namespace CamTest
{
    public partial class Form1 : Form
    {
        private FilterCollection _filters;
        private String dev1, dev2;
        CaptureDevice localSource1, localSource2;
        public int exX = -1, exY = -1;
        //store the calibration img when nothing distorb
       


        #region regIntereset
        //set the points of the region interest
        //Vars for the left camera
        Boolean startSetting = false;
        Point p1=new Point(0,0);
        Point p2=new Point(0,0);
        List<Point> points = new List<Point>();
        Boolean after_first_click = false;
        double tang;
        Boolean storeImg = false;
        Bitmap calIm=null;
 
        //var for the right camera
        Boolean startSettingB = false;
        Point p1B = new Point(0, 0);
        Point p2B = new Point(0, 0);
        List<Point> pointsB = new List<Point>();
        Boolean after_first_clickB = false;
        double tangB;
        Boolean storeImgB = false;
         Bitmap calImB=null;
        #endregion

        public Form1()
        {
            localSource1 = new CaptureDevice();
            localSource2 = new CaptureDevice();
            localSource1.NewFrame+=new CameraEventHandler(localSource1_NewFrame);
            localSource2.NewFrame+=new CameraEventHandler(localSource2_NewFrame);

            InitializeComponent();
            _filters = new FilterCollection(FilterCategory.VideoInputDevice);
            foreach (Filter filter in _filters)
            {
                this.comboBox1.Items.Add(filter.Name);
                this.comboBox2.Items.Add(filter.Name);
            }
            if (_filters.Count == 0)
            {
                this.comboBox1.Items.Add("No capture device found...");
                this.comboBox1.Enabled = false;
                this.comboBox2.Items.Add("No capture device found...");
                this.comboBox2.Enabled = false;
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            
            if (dev1 != null)
            {
                localSource1.VideoSource = dev1;
                if (localSource1 != null)
                {
                    localSource1.Start();
                   
                    
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {

            if (dev2 != null)
            {
                localSource2.VideoSource = dev2;
                if (localSource2 != null)
                {
                    localSource2.Start();
                }
            }
         
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            dev1 = _filters[comboBox1.SelectedIndex].MonikerString;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            dev2 = _filters[comboBox2.SelectedIndex].MonikerString;
        }
        //==========new frame event handling=================================

        private void localSource1_NewFrame(object sender, CameraEventArgs e)
        {
            if (e.Bitmap != null)
            {   
                Bitmap b = e.Bitmap;

               
                if (storeImg)
                {
                    storeCalIm(b,1);
                    storeImg = false;
                }
                changedLocation(b, calIm,1);
                process(b, 1, this);
                
                Image img = b;
                img = resizeImage(img, this.pictureBox1.Size);
                
              
                this.pictureBox1.Image = img;
                this.Update();
                
            }
        }

        private void 
            localSource2_NewFrame(object sender, CameraEventArgs e)
        {
            
            if (e.Bitmap != null)
            {
                Bitmap b = e.Bitmap;
             

                if (storeImgB)
                {
                    storeCalIm(b, 2);
                    storeImgB = false;
                    System.Diagnostics.Debug.WriteLine("store B");
                }
                if (calImB != null)
                {
                  int i=  changedLocation(b, calImB, 2);
                    System.Diagnostics.Debug.WriteLine("DD"+i);
                }
                
                process(b, 2, this);

                Image img = b;
                img = resizeImage(img, this.pictureBox2.Size);

                System.Diagnostics.Debug.WriteLine("2 update");
                this.pictureBox2.Image = img;
                this.Update();

            }
            
        }
  


        //=========imge processing functions=================================

        private static Image resizeImage(Image imgToResize, Size size)
        {
            int sourceWidth = imgToResize.Width;
            int sourceHeight = imgToResize.Height;

            float nPercent = 0;
            float nPercentW = 0;
            float nPercentH = 0;

            nPercentW = ((float)size.Width / (float)sourceWidth);
            nPercentH = ((float)size.Height / (float)sourceHeight);

            if (nPercentH < nPercentW)
                nPercent = nPercentH;
            else
                nPercent = nPercentW;

            int destWidth = (int)(sourceWidth * nPercent);
            int destHeight = (int)(sourceHeight * nPercent);

            Bitmap b = new Bitmap(destWidth, destHeight);
            Graphics g = Graphics.FromImage((Image)b);
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;

            g.DrawImage(imgToResize, 0, 0, destWidth, destHeight);
            g.Dispose();
            return (Image)b;
        }
        //Store calibration image
        public void storeCalIm(Bitmap im,int id)
        {
            System.Diagnostics.Debug.WriteLine("Stre im");
            if (id == 1)
                calIm = new Bitmap(im);
            else
                calImB = new Bitmap(im);
        }

        /*This function suposed to find changes compare to the calibration img which have been stroed before
          When it finds, the corresponding pixel paint in another color, and blue circle appers in the middel of the change  */

        public int changedLocation(Bitmap im, Bitmap cal,int id)
        {
            if (cal == null) return 0;
           
            Graphics g;
            Point _p1, _p2;
            double _tang;
            if (id == 1)
            {
                g = pictureBox1.CreateGraphics();
                _p1 = p1; _p2 = p2;
                _tang = tang;
            }
            else
            {
                _tang = tangB;
                g = pictureBox2.CreateGraphics();
                _p1 = p1B; _p2 = p2B;
            }

            
            //set image from stream dev
            int val = 0, W = im.Width * 3, H = im.Height, cnt = 0;
            BitmapData bd = im.LockBits(new Rectangle(0, 0, im.Width, im.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
           // im.UnlockBits(bd);
            
            int stride = bd.Stride;
            System.IntPtr Scan0 = bd.Scan0;

            
            //set calibration im
            BitmapData Cal_bd = cal.LockBits(new Rectangle(0, 0, im.Width, im.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            int Cal_stride = Cal_bd.Stride;
            System.IntPtr Cal_Scan0 = Cal_bd.Scan0;


            //Thread.Sleep(500);
            
           
            List<Point> ans = new List<Point>();
            Boolean startInterupt = false;
            //int startI = 0, endI = 0;
            Point startI = new Point(0, 0);
            Point endI=new Point(0,0);
            double dy = _p1.Y;
            int y = 0;

            
            
            
            unsafe
                {
                    byte* p = (byte*)(void*)Scan0;
                    byte* Cal_p = (byte*)(void*)Cal_Scan0;

                    for (int x = _p1.X; x <= _p2.X; x++)
                    {
                        dy += _tang;
                        y = (int)Math.Round(dy);
                        int NX = (x * 3 + y * im.Width * 3);

                        if ((Math.Abs((*(p + NX)) - (*(Cal_p + NX))) > 30) || (Math.Abs((*(p + NX + 1)) - (*(Cal_p + NX + 1))) > 30) || (Math.Abs((*(p + NX + 2)) - (*(Cal_p + NX + 2))) > 30))
                        {
                            *(p + NX + 1) = 255;

                            if (!startInterupt)
                            {
                                startInterupt = true;
                                startI = new Point(x, y);
                                endI = new Point(x, y);
                            }

                            else if (startInterupt)
                            {
                                endI = new Point(x, y);
                            }

                        }

                        else //no interupt, which mean end of obsitcle
                        {
                            //if in the last step we had an obsitcle, end it
                            if (startInterupt)
                            {



                                ans.Add(new Point(((endI.X + startI.X) / 2), (endI.Y + startI.Y) / 2));

                                startInterupt = false;
                            }
                        }

                    }

                    for (int i = 0; i < ans.Count; i++)
                    {
                        Point tp = ans.ElementAt(i);
                        g.DrawEllipse(new Pen(Color.Blue), (float)(tp.X - 1.5), (float)(tp.Y + 1.5), 3, 3);

                    }
                    System.Diagnostics.Debug.WriteLine("ID = " + id + "    obsticales =" + ans.Count);
                    calImB.UnlockBits(Cal_bd);
                    im.UnlockBits(bd);
                
            }
                //im.UnlockBits(bd);
                //calIm.UnlockBits(Cal_bd);
            
           
            return 1;
        }

        public static void process(Bitmap im, int camId, Form1 snd)
        {
            int val = 0, W = im.Width * 3, H = im.Height, cnt = 0;
            BitmapData bd = im.LockBits(new Rectangle(0, 0, im.Width, im.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
            int stride = bd.Stride;
            System.IntPtr Scan0 = bd.Scan0;
            int hW = W / 2, hH = H / 2, sz1 = 30, sz2 = 15;

            int maxval = 0, maxcor = -1;

            //snd.pictureBox1.CreateGraphics().DrawLine(new Pen(Color.Black), snd.p1,snd.p2);
            Point _p1, _p2;
            double _tang;
            if (camId == 1)
            {
                _p1 = snd.p1; _p2 = snd.p2;
                _tang = snd.tang;
            }
            else
            {
                _p1 = snd.p1B; _p2 = snd.p2B;
                _tang = snd.tangB;
            }

            unsafe
            {
                byte* p = (byte*)(void*)Scan0;
                
                double dy = _p1.Y;
                int y = 0;
                for (int x = _p1.X; x <= _p2.X; x++)
                {
                    dy += _tang;
                    y = (int)Math.Round(dy);
                    //*(p + (x*3+ y*im.Width*3)*2) = 0;
                    //*(p + (x * 3 + y * im.Width * 3)*2+1) = 0;
                    *(p + (x * 3 + y * im.Width * 3) + 2) = 255;
                }




                im.UnlockBits(bd);
               if (camId==1)
                snd.pictureBox1.CreateGraphics().DrawEllipse(new Pen(Color.Blue, 3), (float)(_p1.X - 1.5), (float)(_p1.Y + 1.5), 3, 3);
                else
                   snd.pictureBox2.CreateGraphics().DrawEllipse(new Pen(Color.Blue, 3), (float)(_p1.X - 1.5), (float)(_p1.Y + 1.5), 3, 3);
            }
        }

           
        

        //======================================================================
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if(localSource1!=null)
            {
                localSource1.Stop();
                localSource1 = null;
            }
            if (localSource2 != null)
            {
                localSource2.Stop();
                localSource2 = null;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            startSetting = true;
        }

        private void button4_MouseClick(object sender, MouseEventArgs e)
        {
            storeImg = true;
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            startSettingB = true;
        }

        private void button6_MouseClick(object sender, MouseEventArgs e)
        {
            storeImgB = true;
        }

        private void pictureBox2_MouseClick(object sender, MouseEventArgs e)
        {
            if (startSettingB)
            {
                if (!after_first_clickB)
                {
                    p1B = new System.Drawing.Point(e.X, e.Y);

                    after_first_clickB = true;
                }
                else
                {
                    p2B = new System.Drawing.Point(e.X, e.Y);
                    after_first_clickB = false;
                    startSettingB = false;

                    tangB = (double)(p2B.Y - p1B.Y) / (double)(p2B.X - p1B.X);

                }
                this.Text = p1B.ToString();
                System.Diagnostics.Debug.WriteLine(p1.ToString());
            }   

        }


        

        
       

    }
}
