/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:41:19 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/16 22:42:58 by hbenazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

Block::Block() : Lvl(0), ArgStart(0), Parent(NULL)
{
}

Block::Block( const Block& copy )
{
    Lvl = copy.Lvl;
    BlockName = copy.BlockName;
    Arg = copy.Arg;
    Blocks = copy.Blocks;
}

Block& Block::operator=( const Block& copy )
{
    if (this != &copy)
    {
        Lvl = copy.Lvl;
        BlockName = copy.BlockName;
        Arg = copy.Arg;
        Blocks = copy.Blocks;
    }
    return (*this);
}

void    Block::GetNames( std::string& s, Block& block )
{
    int         j;
    size_t      pos;
    std::string tmp;
    int         back;

    j = 0;
    while (j < (int)s.length())
    {
        pos = s.find('{', j);
        if (pos != std::string::npos)
        {
            back = pos - 1;
            while ( back >= 0 && s[back] != '{' && s[back] != ';' && s[back] != '}')
                back--;
            back++;
            block.Names.push_back(s.substr(back, pos - back));
            j = pos + 1;
        }
        else
            break;
    }
}

std::vector<Block>&          Block::GetBlocks()
{
    return (Blocks);
}

void        Block::ArgEpur(std::string &s, Block& blk )
{
    int c;

    c = 0;
    size_t Pos;
    GetNames( s,  blk );
    while ( c < (int)blk.Names.size() )
    {
        Pos = blk.Arg.find(blk.Names[c]);
        if ( Pos != std::string::npos )
            blk.Arg.erase(Pos, blk.Names[c].length());
        c++;
    }
}

void     Block::InBrakects( std::string& s, size_t pos, Block &blk )
{
    int c;
    std::string NewString;

    c = 1;
    while ( pos < s.length() )
    {
        if (s[pos] == '{')
            c++;
        else if (s[pos] == '}')
        {
            c--;
            if (c == 0)
                break ;
        }
        else if (c == 1)
            NewString += s[pos] ;
        pos++;
    }
    blk.Arg = NewString;
    if (blk.Lvl > 2)
    {
        std::cerr << "Nested Location detected" << std::endl;
        return ;
    }
    ArgEpur( s, blk );
}

const std::string&   Block::GetName( ) const
{
    return (BlockName);
}

int     Block::GetLvl()
{
    return (Lvl);
}
void    Block::SetBlockName( std::string& s, Block& block, size_t pos )
{
    int         j;

    j = pos;
    while (s[pos])
    {
        if (s[pos] == '{')
        {
            pos--;
            while (pos && (s[pos] != ';' && s[pos] != '{'))
            {
                pos--;
                if (pos == 0)
                    break ;
            }
            if (s[pos] == ';' || s[pos] == '{' || s[pos] == '}')
            {
                while (pos && (s[pos] == ';' || s[pos] == '{' || s[pos] == '}'))
                    pos++;
            }
            block.BlockName = s.substr(pos, j - pos);
            TrimSpaces( block.BlockName );
            block.ArgStart = j + 1;
            return ;
        }
        else
            pos++;
    }
}

bool    Block::HasParent() const
{
    return (Parent != NULL);
}

std::string&    Block::GetArg()
{
    return (Arg);
}


void    Block::FillBlock( std::string& s,Block& block, int& i, int& j )
{
    Block   Child;
    int     pos;
    pos = i;


    while ( s[i] && i < (int)s.length())
    {
        if (s[i] == '}' || s[i] == '{')
        {
            if (s[i] == '{')
            {
                j++;
                Child.Parent = &block;
                Child.Lvl = j;
                SetBlockName( s, Child, i );
                block.Blocks.push_back(Child);
                i++;
                block.Blocks.back().Lvl = j;
                InBrakects( s, Child.ArgStart, block.Blocks.back() );
                FillBlock( s, block.Blocks.back(), i, j );
            }
            else if (s[i] == '}')
            {
                i++;
                j--;
                if (j <= 0)
                    return ;
            }
        }
        else
        {
            while (s[i] && s[i] != '{' && s[i] != '}')
            i++;
            Child.Arg = s.substr(pos, i - pos + 1);
        }
    }
}

Block::~Block()
{
}
