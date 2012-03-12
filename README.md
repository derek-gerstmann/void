													_      __
									 _   __ ____   (_)____/ /
									| | / // __ \ / // __  / 
									| |/ // /_/ // // /_/ /  
									|___/ \____//_/ \__,_/   
										
			
							a visualisation-oriented interface to data


About **void**
==================

**void** is a minimalistic framework designed to support my PhD research and tool development
for large-scale visualisation and analysis.  Even still, it may be useful to others, with the 
caveat that it should be considered somewhat volatile and expiremental until all areas are 
cleaned up and tested.  Currently, this covers a core toolkit, as well as data and 
graphics interfaces.

Primary development environment is OSX 10.7 Lion, so that's the most tested and stable
at this stage.

There is preliminary support for:

* OSX x86/64
* Linux x86/x86_64
* Window x86/64


Distribution
-------------------

Currently **void** is only available in source form via github:

    * git clone git://github.com/voidcycles/void.git


Requirements
-------------------

There are projects provided for **cmake** located under the /projects directory.

You'll need to grab at least cmake v2.8 from Kitware:

    * [cmake] [http://www.cmake.org/]

All other major dependencies are included in header and precompiled binary form
(only for OSX right now -- other platforms soon) under the ../external directory. 

Optional external libraries can be toggled via the **cmake** build tools.


External Dependencies
------------------

Precompiled binaries and headers for most external dependencies are provided in:

    > ./external

For common packages, bash scripts are provided for Unix to build compatible versions
from source.  See:

    > ./scripts/build-*.sh

A external libs are still built manually.  This will be fixed prior to release.


Compiling
------------------

Once all external libraries have been compiled, **cmake** can be used to build platform dependent project files.

For standard usage, the standard **cmake** command-line tools can be used for project generation and compilation steps:

    > mkdir build
    > cd build
    > ccmake ../projects/cmake
    > make

To create an Xcode project file on OSX (for debugging):

    > ccmake -G ../projects/cmake
    > open Void.xcodeproj
    

Test Cases:
----------------

See the ../tests subdirectory for a few stress tests.

        
Mimimal Application Example:
----------------------------

    #include "vd.h"
    #include "runtime/runtime.h"
    
    class MyApp : public vd::Runtim::Application
    {
    ...
    };
    
    int main(int argc, void ** argv)
    {
        MyApp app;
        app.Initialize(&argc, argv);
        return app.Execute();
    }

Documentation
-------------------

* Coming soon.  Lame  -- I know.  Bug me.



