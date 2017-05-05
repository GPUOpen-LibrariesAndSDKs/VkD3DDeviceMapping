Device mapping
==============

Welcome to the device mapping sample - a small example to show how to map devices from different APIs.

License
-------

MIT: see `LICENSE.txt` for details.

System requirements
-------------------

* AMD Radeon&trade; GCN-based GPU (HD 7000 series or newer)
  * Or other Vulkan&trade; compatible discrete GPU 
* 64-bit Windows&reg; 7 (SP1 with the [Platform Update](https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687.aspx)), Windows&reg; 8.1, or Windows&reg; 10
* Visual Studio&reg; 2015
* Graphics driver with Vulkan support
  * For example, AMD Radeon Software Crimson Edition 17.4.4 or later
* The [Vulkan SDK](https://vulkan.lunarg.com) must be installed

Building
--------

Visual Studio files can be found in the `DeviceMapping\build` directory.

If you need to regenerate the Visual Studio files, open a command prompt in the `DeviceMapping\premake` directory and run `..\..\premake\premake5.exe vs2015`.

Third-party software
------------------

* Premake is distributed under the terms of the BSD License. See `premake\LICENSE.txt`.

Attribution
-----------

* AMD, the AMD Arrow logo, Radeon, and combinations thereof are either registered trademarks or trademarks of Advanced Micro Devices, Inc. in the United States and/or other countries.
* Microsoft, Visual Studio, and Windows are either registered trademarks or trademarks of Microsoft Corporation in the United States and/or other countries.
* Linux is the registered trademark of Linus Torvalds in the U.S. and other countries.
* Vulkan and the Vulkan logo are trademarks of the Khronos Group, Inc.
