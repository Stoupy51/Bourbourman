
#include <Multiplayer/Server.hpp>

using std::cerr;
using std::endl;
using std::string;

Server::Server(unsigned short port) : m_context {}, m_clients {},
	m_acceptor {m_context, asio::ip::tcp::endpoint {asio::ip::tcp::v4(), port}}
{
	cerr << "Nouveau serveur !" << endl;
}

void Server::start() {
	// Acceptation des connexions entrantes.
	accept();

	// Démarrage du contexte.
	m_context.run();
}

Server::ClientPtr Server::find(const string & alias) {
	for (ClientPtr client: this->m_clients)
		if(client->alias() == alias)
			return client;
	return nullptr;
}

void Server::accept() {
	m_acceptor.async_accept ([this](const std::error_code & ec, Socket && socket) {
		// Erreur ?
		if (!ec) {
			m_clients.emplace_back(std::make_shared<Client>(this, std::move (socket)));
			m_clients.back()->start();
		}
		accept();
	});
}

void Server::process(ClientPtr client, const string & message) {
	// Lecture d'une éventuelle commande.
	std::istringstream iss (message);
	string command;
	if (iss >> command) {
		// Commande ?
		if (command[0] == '/') {
			// Consommation des caractères blancs.
			iss >> std::ws;
			// Reste du message.
			string data {std::istreambuf_iterator<char> {iss}, std::istreambuf_iterator<char> {}};

			// Recherche du processeur correspondant.
			// - S'il existe, l'appeler ;
			// - Sinon, "#invalid_command" !
			auto search = PROCESSORS.find(command);
			if(search != PROCESSORS.end()) {
				Server::Processor proc = search->second;
				(this->*proc)(client, data);
			}
			else
				client->write(Server::INVALID_COMMAND);
		}
	else
		process_message (client, message);
	}
}

void Server::process_private(ClientPtr client, const string & data) {
	int delimiterPosition = data.find(' ');
	string recipientName = data.substr(0, delimiterPosition);
	string message = data.substr(delimiterPosition + 1, data.length() - 1);

	ClientPtr recipientClient = find(recipientName);
	if(recipientClient != nullptr)
		recipientClient->write("#private " + client->alias() + " " + message);
}

void Server::process_list(ClientPtr client, const string & data) {
	UNUSED(data);
	string m = "#list ";
	for (ClientPtr client: m_clients) {
		m += client->alias() + " ";
		cerr << "-> [" << client->alias() << "]" << endl;
	}
	m.pop_back();
	client->write(m);
}

void Server::process_amogus(ClientPtr client, const string & data) {
	UNUSED(client);
	UNUSED(data);
	broadcast("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀⠀⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣸⣿⡏⠀⠀⠀⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⡄⠀\n"
			"⠀⠀⣀⣤⣴⣶⣶⣿⡟⠀⠀⠀⢸⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀\n"
			"⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿⠀\n"
			"⠀⢸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀\n"
			"⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇⠀⠀\n"
			"⠀⣿⣿⠁⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⠀⠀\n"
			"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n"
			"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀\n"
			"⠀⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀\n"
			"⠀⠸⣿⣧⡀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀\n"
			"⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⣽⣿⡏⠁⠀⠀⢸⣿⡇⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁⠀⠈⠻⣿⣿⣿⣿⡿⠏⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
}

void Server::process_message(ClientPtr client, const string & data) {
	string m = "<b>" + client->alias() + "</b> : " + data;
	broadcast (m);
}

void Server::process_quit(ClientPtr client, const string & data) {
	UNUSED(data);
	client->stop();
}

void Server::broadcast(const string & message, ClientPtr emitter) {
	string m = message + '\n';
	for (ClientPtr client: this->m_clients) {
		if (client != emitter)
		client->write(message);
	}
}

const std::map<string, Server::Processor> Server::PROCESSORS {
	{"/quit", &Server::process_quit},
	{"/list", &Server::process_list},
	{"/private", &Server::process_private},
	{"/ඞ", &Server::process_amogus},
};

const string Server::INVALID_ALIAS		{"#error invalid_alias"};
const string Server::INVALID_COMMAND	{"#error invalid_command"};
const string Server::INVALID_RECIPIENT	{"#error invalid_recipient"};
const string Server::MISSING_ARGUMENT	{"#error missing_argument"};
