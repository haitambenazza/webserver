#include "../headers/webserver.hpp"

std::string    AutoIndex( std::string root, std::string path )
{
    DIR*            Dir;
    struct dirent   *it;
    std::ofstream   index("www/AutoIndex.html");

    // if (path == "/")
    //     path = "";

    if (!index.is_open())
        return("");
    if (path.empty() || root.empty())
        return ( index.close(), "");
    Dir = opendir((root + path).c_str());
    if (Dir == NULL)
        return ( index.close(), root + path);

    index << "<!DOCTYPE html>" << std::endl;
    index << "<html lang=\"en\">" << std::endl;
    index << "<head>" << std::endl;
    index << "    <meta charset=\"UTF-8\">" << std::endl;
    index << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << std::endl;
    index << "    <title>Directory Listing</title>" << std::endl;
    index << "</head>" << std::endl;
    index << "<body>" << std::endl;
    index << "    <h1>Directory Listing</h1>" << std::endl;
    index << "    <ul>" << std::endl;
    it = readdir(Dir);
    if (it == NULL)
        return ( index.close(), "" );

    while ((it = readdir(Dir)) != NULL)
    {
        std::string href;
        if (it->d_type == DT_DIR)
        {
            if (path[path.length() - 1] != '/')
                href = path + "/" + it->d_name;
            else
                href = path + it->d_name;
        }
        else 
        {
            if (path == "/")
                path = "";
            href = path + "/" + it->d_name;
        }
        std::cout << "href == " << href << std::endl;
        index << "      <li><a href=\"" << href << "\">" << it->d_name <<"</a></li>" << std::endl;
    }
    index << "     </ul>" << std::endl;
    index << "</body>" << std::endl;
    index << "</html>" << std::endl;
    closedir(Dir);
    return ("www/AutoIndex.html");
}
