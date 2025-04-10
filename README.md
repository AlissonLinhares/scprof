[![GNU LGPL v3.0](http://www.gnu.org/graphics/lgplv3-88x31.png)](http://www.gnu.org/licenses/lgpl.html)

### About
<p style="text-indent: 2em;" align="justify">
SCProf is an open-source tool for profiling fixed-point and floating-point computations in SystemC models. Different from other applications and libraries used to profile and analyze fixed-point and floating-point operations found in the literature, our solution is capable of detecting underutilized fixed-point operations and providing accurate recommendations and code fixes, including format, range, and precision changes. In addition, our tool is able to report relevant statistics such as how many times each bit has been modified, usage histograms, total reads and writes, numeric ranges for each variable, and many other useful pieces of information to guide engineers in making better decisions in their hardware designs. Moreover, the system was designed to work transparently, allowing SystemC models to be profiled without any source code modifications, and it can be used in combination with other word-length optimizers.
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/AlissonLinhares/scprof/master/doc/scprof0.jpg" width="80%">
</p>
<p style="text-indent: 2em;" align="justify">
SCProf overrides all fixed-point operations within the SystemC library. After running a simulation with representative data, you can use the viewer to analyze what happened inside the application. The tool provides tips on how to improve bit utilization and reduce hardware requirements. The project has been used by IdeaIP to substantially enhance their DSP hardware.
</p>

### Installation
```sh
pip install flask
./install.sh
```

### Configuration
<p style="text-indent: 2em;" align="justify">
Systemc is a dynamic library. To run your application, you need to configure
the runtime path.
</p>
```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/systemc/lib-linux64/
```

### Documentation
Original Paper: <a href="https://github.com/AlissonLinhares/scprof/blob/master/doc/sbcci_2020.pdf">scprof.pdf</a><br>
Presentation slides: <a href="https://github.com/AlissonLinhares/scprof/blob/master/doc/sbcci_pres_2020.pdf">slides.pdf</a><br>

### Demo
See the demo project for details on using the profiler and viewer:
```sh
cd demo
make run
make show
```
<p align="center">
  <img src="https://raw.githubusercontent.com/AlissonLinhares/scprof/master/doc/scprof1.jpg" width="32%">
  <img src="https://raw.githubusercontent.com/AlissonLinhares/scprof/master/doc/scprof2.jpg" width="32%">
  <img src="https://raw.githubusercontent.com/AlissonLinhares/scprof/master/doc/scprof3.jpg" width="32%">
</p>
