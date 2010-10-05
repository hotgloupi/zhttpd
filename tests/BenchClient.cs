using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Net.Sockets;

namespace ConsoleApplication1
{
    class Program
    {
        static int cc = 0;

        static void DoWork()
        {
            byte[] header = Encoding.ASCII.GetBytes("GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
            byte[] buffer = new byte[2048];
            while (true)
            {
                try
                {
                    TcpClient cl = new TcpClient("127.0.0.1", 8080);
                    cl.GetStream().Write(header, 0, header.Length);
                    cl.GetStream().Read(buffer, 0, buffer.Length);
                    cl.Close();
                    int i = Interlocked.Increment(ref cc);
                    if (i % 10 == 0)
                        Console.WriteLine(i);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Error " + e.Message);
                }
            }
        }

        static void Main(string[] args)
        {
            for (int i = 0; i < 1000; ++i)
                new Thread(DoWork).Start();
            DoWork();
        }
    }
}
