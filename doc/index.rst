Welcome to sequencer-plugin-mathexpr's documentation!
=====================================================

   .. toctree::
      :maxdepth: 2
      :caption: Contents:

   `sequencer-plugin-mathexpr` is a sequencer plugin that provides functionality for processing mathematical expressions using the exprtk library.

Functionalities
---------------
sequencer-plugin-mathexpr provides the following functionalities for processing mathematical expressions:

1. Supports variables of any numeric type supported by AnyValue
2. Supports arrays and scalars as variables
3. Automatically converts variables to doubles before processing the expression
4. Automatically converts the result back to the input type after processing the expression

Usage
-----

To use this plugin it needs to be compiled and installed in the system. The following line has to be added to the xml:

.. code-block:: xml

   <Plugin>libsequencer-mathexpr.so</Plugin>

The most basic use for this plugin is to evaluate mathematical expressions of scalars:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="z:=x+y-1"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint8"}' value='1' />
        <Local name="y" type='{"type":"uint8"}' value='1' />
        <Local name="z" type='{"type":"uint8"}' value='0' />
    </Workspace>


In this example the output of the is saved in the variable "z". All non logical mathematical expressions need to be assigned to an output variable.
The type of the output is always respected, so if the example above the result of the expression is negative or above the maximum value possible for an uint8, the sequencer will return an executionstatus of failure.

Arrays can be used:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="z:=x+y*y-2"/>
        <Equals leftVar="z" rightVar="a"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
        <Local name="y" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[1,1,1]"/>
        <Local name="z" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[0,0,0]"/>
        <Local name="a" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[1,3,5]"/>
    </Workspace>

More than one assignment, even if chained, can be done in one call:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="y:=x+1; z:=y+2"/>
        <Equals leftVar="y" rightVar="a"/>
        <Equals leftVar="z" rightVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"float32"}' value='158'/>
        <Local name="y" type='{"type":"float32"}' value='0'/>
        <Local name="z" type='{"type":"float32"}' value='0'/>
        <Local name="a" type='{"type":"float32"}' value='159'/>
        <Local name="b" type='{"type":"float32"}' value='161'/>
    </Workspace>


In logical expressions an assignment in not mandatory:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="x > y^2"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>

But it can be attributed:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="b:=x > y^2"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
        <Local name="b" type='{"type":"bool"}' value='false' />
    </Workspace>

An expression can be passed from a string variable:

.. code-block:: xml

    <Sequence>
        <MathExpression expression="@cond"/>
    </Sequence>
    <Workspace>
        <Local name="cond" type='{"type":"string"}' value='"x > y^2"' />
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
