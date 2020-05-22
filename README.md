[![GNU LGPL v3.0](http://www.gnu.org/graphics/lgplv3-88x31.png)](http://www.gnu.org/licenses/lgpl.html)

### About
<p style="text-indent: 2em;" align="justify">
In this work we present an open source tool for profiling fixed-point and floating-point computations in SystemC models, the SCProf. Different from other applications and libraries used to profile and analyze fixed-point and floating-point operations, found in the literature, our solution is capable of detecting underutilized fixed-point operations, providing accurate recommendations and code fixes, including format, range and precision changes. In addition, our tools are able to report relevant statistics such as: how many times each bit has been modified, usage histograms, total reads and writes, numeric ranges for each variable and many other useful information to guide engineers to make better decisions in their hardware designs. Moreover, the system was designed to work transparently, allowing models written in SystemC to be profiled without any source code modification and can be used in combination with other word-length optimizers.
</p>

### Installation
```sh
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
