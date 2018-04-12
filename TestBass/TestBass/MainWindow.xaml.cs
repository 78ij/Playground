using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using Un4seen.Bass;
using Un4seen.Bass.Misc;
using Un4seen.Bass.AddOn.Wma;
using SharpDX.Direct2D1;
using System.Threading;

using System.Timers;
using OpenFileDialog = Microsoft.Win32.OpenFileDialog;
using MessageBox = System.Windows.MessageBox;

namespace TestBass
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        System.Windows.Forms.Timer globaltimer = new System.Windows.Forms.Timer()
        {
            Interval = 500
        };
        System.Timers.Timer FFTtimer = new System.Timers.Timer(20);
        string CurrentMusicPath;
        IntPtr hwnd;
        bool playing = false;
        int stream = 0;
        float oldvalue = 0;
        private void TimerUpd(object sender,EventArgs e)
        {
            Time.Content = SecondtoMinutes(Bass.BASS_ChannelBytes2Seconds(stream,Bass.BASS_ChannelGetPosition(stream)));
            RTime.Content = "-" + SecondtoMinutes(Bass.BASS_ChannelBytes2Seconds(stream, Bass.BASS_ChannelGetLength(stream) - 
                Bass.BASS_ChannelGetPosition(stream)));
            TimeS.Value = (double)Bass.BASS_ChannelGetPosition(stream)
                / (double)Bass.BASS_ChannelGetLength(stream) * 10;
        }

        void SelectFilePath()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog()
            {
                Filter = "Playable Files |*.wav;*.mp3"
            };
            var res = openFileDialog.ShowDialog();
            if (res == true)
            {
                CurrentMusicPath = openFileDialog.FileName;
            }
        }

        public MainWindow()
        {
            InitializeComponent();
            TimeS.AddHandler(Slider.MouseLeftButtonUpEvent, new MouseButtonEventHandler(TimeS_MouseLeftUp),true);
            TimeS.AddHandler(Slider.MouseLeftButtonDownEvent, new MouseButtonEventHandler(TimeS_MouseLeftDown), true);
            TimeS.AddHandler(Slider.MouseMoveEvent, new System.Windows.Input.MouseEventHandler(TimeS_Drag), true);
            TimeS.AddHandler(Slider.PreviewMouseDownEvent, new System.Windows.Input.MouseButtonEventHandler(TimeS_Down), true);
            globaltimer.Enabled = false;
            FFTtimer.Enabled = false;
            FFTtimer.Elapsed += new ElapsedEventHandler(FFTupd);
            Un4seen.Bass.BassNet.Registration("x739566858@outlook.com", "2X47302224822");
            hwnd = new WindowInteropHelper(this).Handle;
            Bass.BASS_Init(-1, 44100, BASSInit.BASS_DEVICE_DEFAULT, hwnd);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Bass.BASS_Free();
            FFTtimer.Enabled = false;
            globaltimer.Enabled = false;
            MessageBox.Show("Closing!");
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!playing)
            {
                if (CurrentMusicPath != null)
                {
                    stream = Bass.BASS_StreamCreateFile(CurrentMusicPath, 0, 0, BASSFlag.BASS_DEFAULT);
                    if (stream != 0)
                    {
                        Bass.BASS_ChannelPlay(stream, false);
                        playing = true;
                        globaltimer.Enabled = true;
                        FFTtimer.Enabled = true;
                        globaltimer.Tick += TimerUpd;
                    }
                }
            }
            else
            {
                Bass.BASS_ChannelStop(stream);
                Bass.BASS_StreamFree(stream);
                stream = 0;
                playing = false;
                FFTtimer.Enabled = false;
                globaltimer.Enabled = false;
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            SelectFilePath();
            if (CurrentMusicPath != null)
            {
                MusicLabel.Content = "Playing: " + CurrentMusicPath;
            }
        }

        private void TimeS_MouseLeftUp(object sender, MouseButtonEventArgs e)
        {

            double ratio = TimeS.Value / 10.0;
            long length = Bass.BASS_ChannelGetLength(stream);
            length = (long)(length * ratio);
            Bass.BASS_ChannelSetPosition(stream, length);
            globaltimer.Enabled = true;
            FFTtimer.Enabled = true;
            TimerUpd(null, null);

        }
        
        private void TimeS_MouseLeftDown(object sender,MouseButtonEventArgs e)
        {
            globaltimer.Enabled = false;
            FFTtimer.Enabled = false;
        }

        private void TimeS_Down(object sender, MouseButtonEventArgs e)
        {
            globaltimer.Enabled = false;
            FFTtimer.Enabled = false;
        }

        private void TimeS_Drag(object sender, System.Windows.Input.MouseEventArgs e)
        {
            double ratio = TimeS.Value / 10.0;
            long length = Bass.BASS_ChannelGetLength(stream);
            length = (long)(length * ratio);
            Time.Content = SecondtoMinutes(Bass.BASS_ChannelBytes2Seconds(stream, Bass.BASS_ChannelGetLength(stream)) * ratio);
            RTime.Content = "-" + SecondtoMinutes(Bass.BASS_ChannelBytes2Seconds(stream, Bass.BASS_ChannelGetLength(stream))
                - Bass.BASS_ChannelBytes2Seconds(stream, Bass.BASS_ChannelGetLength(stream)) * ratio);
        }

        
        public void FFT()
        {
            if (playing)
            {
                int length = (int)(sizeof(float) * 256);
                float[] data = new float[256];
                GCHandle hGC = GCHandle.Alloc(data, GCHandleType.Pinned);
                length = Bass.BASS_ChannelGetData(stream, hGC.AddrOfPinnedObject(), (int)BASSData.BASS_DATA_FFT256);
                int level = (Bass.BASS_ChannelGetLevel(stream) >> 16) & 0x0000FFFF;

                float fres = data[(int)(length * 200 / 44100)] + data[(int)(length * 150 / 44100)] + data[(int)(length * 100 / 44100)];
                fres /= 3;
                hGC.Free();
                Lb.Content = fres.ToString();
                //if (fres > oldvalue) oldvalue = fres;
                //Lb.Content = oldvalue.ToString();
                if (!float.IsNaN(fres))
                {
                    TimeS1.Value = Math.Abs((double)level / 32768.0) * 10;
                    double red = (fres * 255) * 3;
                    Color color = new Color()
                    {
                        R = (Byte)(255 - red),
                        G = (Byte)(255 - red),
                        B = (Byte)(255 - red),
                        A = 255
                    };
                    Rect.Fill = new System.Windows.Media.SolidColorBrush(color);
                    
                }

            }
        }

        private void FFTupd(object sender, System.Timers.ElapsedEventArgs e) {
            Dispatcher.Invoke(new Action(FFT));
        }

        static string SecondtoMinutes(double second)
        {   
            
            int minutes = (int)(second / 60);
            int seconds = (int)second - (minutes * 60);
            string minutest = minutes.ToString();
            string secondst = seconds.ToString();
            if (minutest.Length == 1)
            {
                minutest = "0" + minutest;
            }
            if (secondst.Length == 1)
            {
                secondst = "0" + secondst;
            }
            string ret = minutest + ":" + secondst;
            return ret;
        }
    }
}
