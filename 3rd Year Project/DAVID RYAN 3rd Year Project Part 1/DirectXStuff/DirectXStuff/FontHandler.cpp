
#include "FontHandler.h"
//DONE BEFORE I KNEW ABOUT A THING CALLED DirectWrite

FontHandler::FontHandler()
{
	_Font = 0;
	_Texture = 0;
}




FontHandler::~FontHandler()
{
}

bool FontHandler::Init(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in text file containing font data.
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// Load texture that has font characters on it.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void FontHandler::KillSafely()
{
	ReleaseTexture();

	ReleaseFontData();

	return;
}

bool FontHandler::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create font spacing buffer.
	_Font = new FontType[95];
	
	// Read in font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> _Font[i].left;
		fin >> _Font[i].right;
		fin >> _Font[i].size;
	}

	// Close file.
	fin.close();

	return true;
}

void FontHandler::ReleaseFontData()
{
	if(_Font)
	{
		delete [] _Font;
		_Font = 0;
	}

	return;
}

bool FontHandler::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	_Texture = new Texture;

	result = _Texture->Init(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void FontHandler::ReleaseTexture()
{
	if(_Texture)
	{
		_Texture->KillSafely();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontHandler::GetTexture()
{
	return _Texture->GetTexture();
}

void FontHandler::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	Vertex* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce input vertices into a Vertex structure.
	vertexPtr = (Vertex*)vertices;

	// Get number of letters in sentence.
	numLetters = (int)strlen(sentence);

	// Init index to vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + _Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(_Font[letter].right, 1.0f);
			index++;

			// Update x location for drawing by size of letter and one pixel.
			drawX = drawX + _Font[letter].size + 1.0f;
		}
	}

	return;
}


