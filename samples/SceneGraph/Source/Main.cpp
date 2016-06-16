#include <Sauce/Sauce.h>

using namespace sauce;

/**
 * This example construts a simple scene graph and outputs two Graphviz (.gv) files.
 *
 * To save the graphs as PNGs, use the commands:
 * dot -Tpng SceneGraph.gv -o SceneGraph.png
 * dot -Tpng SceneGraph_Order.gv -o SceneGraph_Order.png
 *
 * SceneGraph.gv: A graph showing the scene graph. The nodes in the graph have their events called in a pre-order fashion.
 * SceneGraph_Order.gv: A graph showing the order of which the SceneObjects will have their evens called.
 */
int depth = 0;
class SceneGraphGame : public Game
{
public:
	SceneGraphGame() :
		Game("SceneGraph")
	{
	}

	void exportToGraphviz(string outpath)
	{
		// Nodes and edges
		string nodes = "\tnode [shape=box];";
		string sgedges, oredges;
		SceneObject *prevNode = 0;

		// Go through nodes using DFS
		deque<SceneObject*> Q;
		Q.push_front(getScene()->getRoot());
		while(!Q.empty())
		{
			// Extract first node
			SceneObject *node = Q.front();
			Q.pop_front();

			// Add node to list
			nodes += " " + string((char*) node->getUserData()) + ";";

			// Add order edges
			if(prevNode)
			{
				oredges += "\t" + string((char*) prevNode->getUserData()) + "->" + string((char*) node->getUserData()) + "\n";
			}
			prevNode = node;

			// Add tree edges
			list<SceneObject*> children = node->getChildren();
			for(SceneObject *child : children)
			{
				sgedges += "\t" + string((char*) node->getUserData()) + "->" + string((char*) child->getUserData()) + "\n";
			}

			// Insert children at start of the queue
			Q.insert(Q.begin(), children.begin(), children.end());
		}

		// Write SceneGraph tree
		ofstream sgfile(util::getAbsoluteFilePath(outpath + ".gv"));
		if(!sgfile)
		{
			LOG("Unable to open file.");
			return;
		}

		sgfile << "digraph SceneGraph" << endl;
		sgfile << "{" << endl;
		sgfile << nodes << endl;
		sgfile << sgedges;
		sgfile << "}" << endl;
		sgfile.close();

		// Write traversal order to file
		ofstream orfile(util::getAbsoluteFilePath(outpath + "_Order.gv"));
		if(!orfile)
		{
			LOG("Unable to open file.");
			return;
		}

		orfile << "digraph SceneGraph" << endl;
		orfile << "{" << endl;
		orfile << nodes << endl;
		orfile << oredges;
		orfile << "}" << endl;
		orfile.close();
	}

	class CustomSceneObject : public SceneObject
	{
		void onTick(TickEvent *e)
		{
			depth++;
			string tabStr;
			for(int i = 0; i < depth; ++i) tabStr += "\t";
			LOG("%s%s onTick", tabStr.c_str(), (char*) getUserData());
			SceneObject::onTick(e);
			depth--;
		}
	};

	void onStart(GameEvent*)
	{
		Scene *scene = getScene();
		SceneObject *root = scene->getRoot();
		root->setUserData("Root");

		SceneObject *gui = new CustomSceneObject();
		gui->setUserData("GUI");
		root->addChildFirst(gui);
		
		// GUI
		{
			SceneObject *scrollArea = new CustomSceneObject();
			scrollArea->setUserData("ScrollArea");
			gui->addChildFirst(scrollArea);

			SceneObject *button = new CustomSceneObject();
			button->setUserData("Button");
			gui->addChildFirst(button);

			// ScrollArea
			{
				SceneObject *image = new CustomSceneObject();
				image->setUserData("Image");
				scrollArea->addChildLast(image);

				SceneObject *radioBox1 = new CustomSceneObject();
				radioBox1->setUserData("RadioBox1");
				scrollArea->addChildLast(radioBox1);

				SceneObject *radioBox2 = new CustomSceneObject();
				radioBox2->setUserData("RadioBox2");
				scrollArea->addChildLast(radioBox2);
			}
		}

		SceneObject *game = new CustomSceneObject();
		game->setUserData("Game");
		root->addChildFirst(game);

		// Game
		{
			SceneObject *enemy = new CustomSceneObject();
			enemy->setUserData("Enemy");
			game->addChildFirst(enemy);

			SceneObject *player = new CustomSceneObject();
			player->setUserData("Player");
			game->addChildFirst(player);

			SceneObject *background = new CustomSceneObject();
			background->setUserData("Background");
			game->addChildFirst(background);

			SceneObject *camera = new CustomSceneObject();
			camera->setUserData("Camera");
			game->addChildFirst(camera);
		}
		
		exportToGraphviz(":/SceneGraph");
	}

	void onEnd(GameEvent*)
	{
	}

	void onTick(TickEvent *e)
	{
		LOG("%s onTick", (char*) getUserData());
		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	SceneGraphGame game;
	return game.run();
}