using MathNet.Numerics.LinearAlgebra;
using System;
using System.IO.Ports;
using System.Windows;

namespace BSS {
    public partial class MainWindow : Window {

        private class SensorModel : KalmanModel {

            public SensorModel() {
                double ts = 0.004;

                F = Matrix<double>.Build.DenseOfArray(new double[,] {
                    { 1, ts, ts * ts / 2.0 },
                    { 0, 1, ts },
                    { 0, 0, 1 }
                });

                B = Matrix<double>.Build.DenseOfArray(new double[,] {
                    { 0 },
                    { 0 },
                    { 0 }
                });

                H = Matrix<double>.Build.DenseOfArray(new double[,] {
                    { 1, 0, 0 }
                });

                Q = Matrix<double>.Build.DenseOfArray(new double[,] {
                    { 1, 0, 0 },
                    { 0, 50, 0 },
                    { 0, 0, 100 }
                });

                R = Matrix<double>.Build.DenseOfArray(new double[,] {
                    { 15 }
                });
            }

        }

        private readonly KalmanFilter filter;

        private readonly SerialPort serialPort;

        public MainWindow() {
            InitializeComponent();

            chart.Title = "Distance";
            chart.RefreshDelay = 200;
            chart.AddSeries("Distance [mm]", "Distance", true);
            chart.AddSeries("Filtered distance [mm]", "Filtered distance [mm]", true);

            filter = new KalmanFilter(new SensorModel());
            serialPort = new SerialPort {
                BaudRate = 115200
            };

            portSelect.Items.Add("- Port -");
            portSelect.SelectedIndex = 0;

            portSelect.DropDownOpened += (s, e) => {
                portSelect.Items.Clear();
                portSelect.Items.Add("- Port -");
                var ports = SerialPort.GetPortNames();

                foreach (var port in ports) {
                    portSelect.Items.Add(port);
                }
            };

            portSelect.SelectionChanged += (s, e) => {
                if (portSelect.SelectedItem != null) {
                    try {
                        serialPort.PortName = portSelect.SelectedItem.ToString();
                        serialPort.Open();
                        serialPort.DiscardInBuffer();
                        portSelect.IsEnabled = false;
                        disconnectBtn.IsEnabled = true;
                    } catch (Exception ex) {
                        Console.WriteLine($"Error: {ex.Message}");
                    }
                }
            };

            disconnectBtn.Click += (s, e) => serialPort.Close();
            startBtn.Click += (s, e) => {
                //TODO:
            };
            stopBtn.Click += (s, e) => {
                //TODO:
            };

            serialPort.Disposed += (s, e) => {
                portSelect.IsEnabled = true;
                disconnectBtn.IsEnabled = false;
                startBtn.IsEnabled = false;
                stopBtn.IsEnabled = false;
            };

            serialPort.DataReceived += (s, e) => {
                try {
                    double rawData = Convert.ToDouble(serialPort.ReadLine());

                    if (rawData > 200) {
                        return;
                    }

                    double filteredData = filter.Compute(rawData)[0];

                    chart.Update(new double[] { rawData, filteredData });
                } catch (Exception) {
                }
            };
        }

    }
}