#include <CGF/CGF.h>

using namespace cgf;

/**
 * This example construts a simple scene graph and outputs two Graphviz (.gv) files.
 *
 * To save the graphs as PNGs, use the commands:
 * dot -Tpng SceneGraph.gv -o SceneGraph.png
 * dot -Tpng SceneGraph_Order.gv -o SceneGraph_Order.png
 *
 * SceneGraph.gv: A graph showing the scene graph. The nodes in the graph have their events called in a pre-order fashion.
 * SceneGraph_Order.gv: A graph showing the order of which the GameObjects will have their evens called.
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
		GameObject *prevNode = 0;

		// Go through nodes using DFS
		deque<GameObject*> Q;
		Q.push_front(getScene()->getRoot());
		while(!Q.empty())
		{
			// Extract first node
			GameObject *node = Q.front();
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
			list<GameObject*> children = node->getChildren();
			for(GameObject *child : children)
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

	class CustomGameObject : public GameObject
	{
		void onTick(TickEvent *e)
		{
			depth++;
			string tabStr;
			for(int i = 0; i < depth; ++i) tabStr += "\t";
			LOG("%s%s onTick", tabStr.c_str(), (char*) getUserData());
			GameObject::onTick(e);
			depth--;
		}
	};

	void onStart(GameEvent*)
	{
		Scene *scene = getScene();
		GameObject *root = scene->getRoot();
		root->setUserData("Root");

		GameObject *gui = new CustomGameObject();
		gui->setUserData("GUI");
		root->addChildFirst(gui);
		
		// GUI
		{
			GameObject *scrollArea = new CustomGameObject();
			scrollArea->setUserData("ScrollArea");
			gui->addChildFirst(scrollArea);

			GameObject *button = new CustomGameObject();
			button->setUserData("Button");
			gui->addChildFirst(button);

			// ScrollArea
			{
				GameObject *image = new CustomGameObject();
				image->setUserData("Image");
				scrollArea->addChildLast(image);

				GameObject *radioBox1 = new CustomGameObject();
				radioBox1->setUserData("RadioBox1");
				scrollArea->addChildLast(radioBox1);

				GameObject *radioBox2 = new CustomGameObject();
				radioBox2->setUserData("RadioBox2");
				scrollArea->addChildLast(radioBox2);
			}
		}

		GameObject *game = new CustomGameObject();
		game->setUserData("Game");
		root->addChildFirst(game);

		// Game
		{
			GameObject *enemy = new CustomGameObject();
			enemy->setUserData("Enemy");
			game->addChildFirst(enemy);

			GameObject *player = new CustomGameObject();
			player->setUserData("Player");
			game->addChildFirst(player);

			GameObject *background = new CustomGameObject();
			background->setUserData("Background");
			game->addChildFirst(background);

			GameObject *camera = new CustomGameObject();
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