## HOW TO USE


#### Setup ImperiumDB Project

Follow instructions here on setting up the imperium DB project. NOTE: It is strongly recommended you place the project inside your games `Content` folder.


#### Project Settings

Install and activate the plugin in Unreal Engine.

Create a new asset by opening `Misc>DataAsset` and look for `Imperium Project Settings`.

Go into `Project Setting>Imperium DB` and add your new project settings asset.

Set the path to your `.ImpProject` file. Note there are some keywords that can act as path shortcuts.

* `{project}`: Path to your Project/Game folder.
* `{content}`: Path to your Project/Game's CONTENT folder.

Imperium will now automatically try and load `ImpFlow` assets by their name, regardless of path.

#### Node Settings

Here you will configure what blueprint nodes will run when certain nodes in Imperium DB are hit.

* `def class` : the default class that will be run for all nodes. 
* `start node`: The template node that will first be searched for as the starting node. This by default is `node_Start`.
* `temp override`: ImpNode blueprint classes that will attemtped to be used instead of the default for certain Imperium Node templates names. 


### Nodes and Instances

`Imperium Instances` & `Imperium Nodes` are spawned actor that handle how Imperium DB is implemented for your specific unreal engine project.

##### - Instance Blueprint


##### - Node Blueprint


#### Runing an `.ImpFlow` 

To play an Impflow can be done a few ways, but the easiest us Loading a flow and using the async blueprint task to play it. In this blueprint node, you can choose an `Imperium Instance` class to play for this instance, OR if none is set it will use the default class from your `Node Settings` asset.