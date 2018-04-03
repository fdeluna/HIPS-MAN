#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <iostream>
#include <Importer.h>

using namespace std;
using namespace xercesc;

template<> Importer* Ogre::Singleton<Importer>::msSingleton = 0;

Importer* Importer::getSingletonPtr(void)
{
    return msSingleton;
}
Importer& Importer::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}

void Importer::parseScene (const char* path, Scene *scene)
{
  // Inicialización.
  try {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Error durante la inicialización! :\n"
	 << message << "\n";
    XMLString::release(&message);
    return;
  }

  XercesDOMParser* parser = new XercesDOMParser();
  parser->setValidationScheme(XercesDOMParser::Val_Always);

  // 'Parseando' el fichero xml...
  try {
    parser->parse(path);
  }
  catch (const XMLException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Excepción capturada: \n"
	 << message << "\n";
    XMLString::release(&message);
  }
  catch (const DOMException& toCatch) {
    char* message = XMLString::transcode(toCatch.msg);
    cout << "Excepción capturada: \n"
	 << message << "\n";
    XMLString::release(&message);
  }
  catch (...) {
    cout << "Excepción no esperada.\n" ;
    return;
  }

  DOMDocument* xmlDoc;
  DOMElement* elementRoot;

  try {
    // Obtener el elemento raíz del documento.
    xmlDoc = parser->getDocument(); 
    elementRoot = xmlDoc->getDocumentElement();

    if(!elementRoot)
      throw(std::runtime_error("Documento XML vacío."));

  }
  catch (xercesc::XMLException& e ) {
    char* message = xercesc::XMLString::transcode( e.getMessage() );
    ostringstream errBuf;
    errBuf << "Error 'parseando': " << message << flush;
    XMLString::release( &message );
    return;
  }

  XMLCh* camera_ch = XMLString::transcode("camera");  
  XMLCh* graph_ch = XMLString::transcode("graph");  

  // Procesando los nodos hijos del raíz...
  for (XMLSize_t i = 0; 
       i < elementRoot->getChildNodes()->getLength(); 
       ++i ) {
    
    DOMNode* node = elementRoot->getChildNodes()->item(i);

    if (node->getNodeType() == DOMNode::ELEMENT_NODE) {      
	// Nodo <graph>?
	if (XMLString::equals(node->getNodeName(), graph_ch))
	  parseGraph(node, scene);
    }
    
  }
  // Liberar recursos.
  XMLString::release(&camera_ch);
  XMLString::release(&graph_ch);

  delete parser;
}

void Importer::parseGraph (xercesc::DOMNode* graphNode, Scene* scene)
{
  XMLCh* vertex_ch = XMLString::transcode("vertex");
  XMLCh* edge_ch = XMLString::transcode("edge");

  for (XMLSize_t i = 0; i < graphNode->getChildNodes()->getLength(); ++i ) {

    DOMNode* node = graphNode->getChildNodes()->item(i);

    if (node->getNodeType() == DOMNode::ELEMENT_NODE) {
      // Nodo <vertex>?
      if (XMLString::equals(node->getNodeName(), vertex_ch))
	addVertexToScene(node, scene);
      else
      // Nodo <edge>?
	if (XMLString::equals(node->getNodeName(), edge_ch)) {
	  addEdgeToScene(node, scene);
	}
    }
    
  }

  XMLString::release(&vertex_ch);
  XMLString::release(&edge_ch);
}

void Importer::addVertexToScene (xercesc::DOMNode* vertexNode, Scene* scene)
{
  DOMNamedNodeMap* attributes = vertexNode->getAttributes();
  DOMNode* indexNode = attributes->getNamedItem(XMLString::transcode("index"));
  DOMNode* typeNode = attributes->getNamedItem(XMLString::transcode("type"));

  int vertex_index = atoi(XMLString::transcode(indexNode->getNodeValue()));
  string vertex_type = XMLString::transcode(typeNode->getNodeValue());  

  XMLCh* x_ch = XMLString::transcode("x");  
  XMLCh* y_ch = XMLString::transcode("y");  
  XMLCh* z_ch = XMLString::transcode("z");  

  float x = getValueFromTag(vertexNode, x_ch);
  float y = getValueFromTag(vertexNode, y_ch);
  float z = getValueFromTag(vertexNode, z_ch);

  // Instanciar la posición del nodo.
  Ogre::Vector3 position(x, y, z);
  // Instanciar el nodo.
  std::cout << vertex_type << std::endl;
  Node n(vertex_index, vertex_type, position);
  // Añadir el nodo a la estructura de grafo.
  scene->getGraph()->addVertex(new GraphVertex(n));

  XMLString::release(&x_ch);
  XMLString::release(&y_ch);
  XMLString::release(&z_ch);
}

void Importer::addEdgeToScene (xercesc::DOMNode* edgeNode, Scene* scene)
{
  XMLCh* vertex_ch = XMLString::transcode("vertex");
  std::vector<int> edge;

  for (XMLSize_t i = 0; i < edgeNode->getChildNodes()->getLength(); ++i ) {

    DOMNode* node = edgeNode->getChildNodes()->item(i);
    // Nodo <vertex>?
    if ((node->getNodeType() == DOMNode::ELEMENT_NODE) &&
  	XMLString::equals(node->getNodeName(), vertex_ch))
      edge.push_back(atoi(XMLString::transcode
			  (node->getFirstChild()->getNodeValue())));
  }

  // Recuperar los vértices a partir de su index,
  // para generar y añadir el arco.
  GraphVertex *v1 = scene->getGraph()->getVertex(edge[0]);
  GraphVertex *v2 = scene->getGraph()->getVertex(edge[1]);

  if (v1 && v2)
  {
	  scene->getGraph()->addEdge(v1, v2);
  }

  XMLString::release(&vertex_ch);
}

float Importer::getValueFromTag (xercesc::DOMNode* node, const XMLCh *tag)
{
  for (XMLSize_t i = 0; i < node->getChildNodes()->getLength(); ++i ) {

    DOMNode* aux = node->getChildNodes()->item(i);

    if (aux->getNodeType() == DOMNode::ELEMENT_NODE &&
	XMLString::equals(aux->getNodeName(), tag))
      return atof(XMLString::transcode(aux->getFirstChild()->getNodeValue()));

  }
  return 0.0;
}
