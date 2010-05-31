--[[
Shows the server widget when first connected to a server. In this widget the user is able to login to the server, or create a new account if none yet exists.
After logging it the user can either use an existing character, or create a new one.
If the user is an admin a button is shown allowing the user to log in as a creator character.

Note that since the widget currently is coded in C++ we'll use the createWidget(...) method of creating it. 
]]--

Server = {connectors={}}

--Show the server widget when a new connection is achieved.
connect(Server.connectors, emberServices:getServerService().GotConnection, function(connection)
		guiManager:createWidget("ServerWidget")
	end
)