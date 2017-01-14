function init(){
    Cipherinator.addCipher("Атбаш", atbash, "Шифр Атбаш");
    Cipherinator.addCipher("Шифр Цезаря", caesar, "Шифр Цезаря\nДля указания сдвига напишите число в поле Ключ");
    Cipherinator.addCipher("Шифр Виженера", vigenere, "Шифр Виженера");
}
function atbash(text, alphabet){
    if(text == "") return "";

    ret = "";
    for(i = 0; i < text.length; i++)
        if(alphabet.indexOf(text[i].toLowerCase()) > -1){
            ch = alphabet[(alphabet.length - alphabet.indexOf(text[i].toLowerCase()) - 1) % alphabet.length];
            if(text[i].toLowerCase() != text[i]) ch = ch.toUpperCase();
            ret += ch;
        } else ret += text[i];
    return ret;
}
function caesar(text, alphabet, decrypt, key){
    if(text == "") return "";

    rot = parseInt(key, 10);
    if(rot != rot) rot = 3;

    if(decrypt) rot = alphabet.length - rot;

    ret = "";
    for(i = 0; i < text.length; i++)
         if(alphabet.indexOf(text[i].toLowerCase()) > -1){
            ch = alphabet[(alphabet.indexOf(text[i].toLowerCase()) + rot) % alphabet.length];
            if(text[i].toLowerCase() != text[i]) ch = ch.toUpperCase();
            ret += ch;
        } else ret += text[i];

    return ret;
}
function vigenere(text, alphabet, decrypt, key){
    if(text == "") return "";
    if(key == "") return text;

    key = key.toLowerCase();
    tmpkey = key;
    while(key.length < text.length) key += tmpkey;

    ret = "";
    keyi = 0;
    for(i = 0; i < text.length; i++)
        if(alphabet.indexOf(text[i].toLowerCase()) > -1){
            ch = "";
            if(!decrypt) ch = alphabet[(alphabet.indexOf(text[i].toLowerCase()) + alphabet.indexOf(key[keyi])) % alphabet.length];
            else ch = alphabet[(alphabet.indexOf(text[i].toLowerCase()) - alphabet.indexOf(key[keyi]) + alphabet.length) % alphabet.length];
            if(text[i].toLowerCase() != text[i]) ch = ch.toUpperCase();
            ret += ch;
            keyi++;
        } else ret += text[i];

    return ret;
}
