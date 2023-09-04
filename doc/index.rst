Welcome to sequencer-plugin-math's documentation!
==================================================


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   `sequencer-plugin-math` is a sequencer plugin that provides functionality for processing mathematical expressions using the exprtk library.

Functionalities
---------------
sequencer-plugin-math provides the following functionalities for processing mathematical expressions:

1. Supports variables of any numeric type supported by AnyValue
2. Supports arrays and scalars as variables
3. Automatically converts variables to doubles before processing the expression
4. Automatically converts the result back to the input type after processing the expression

Usage
-----

To use this plugin it needs to be compiled and installed in the system. The following line has to be added to the xml:


.. code-block:: xml

   <Plugin>libsequencer-math.so</Plugin>



   
Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
