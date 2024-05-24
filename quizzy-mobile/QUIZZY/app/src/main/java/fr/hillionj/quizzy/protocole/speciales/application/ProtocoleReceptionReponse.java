package fr.hillionj.quizzy.protocole.speciales.application;

import fr.hillionj.quizzy.protocole.Protocole;
import fr.hillionj.quizzy.protocole.TypeProtocole;

@SuppressWarnings({ "SpellCheckingInspection", "unused" })
public class ProtocoleReceptionReponse extends Protocole
{
    public ProtocoleReceptionReponse(String trame)
    {
        setTrame(trame);
    }

    @Override
    public String getFormat()
    {
        return "$" + getType().getIndiceType() + ";QUESTION;REPONSE;TEMPS\n";
    }

    @Override
    public TypeProtocole getType()
    {
        return TypeProtocole.RECEPTION_REPONSE;
    }

    public long getTempsReponse()
    {
        return toLong(extraireDonnees().get("TEMPS"));
    }

    public int getNumeroReponse()
    {
        return toInt(extraireDonnees().get("REPONSE"));
    }

    public int getNumeroQuestion()
    {
        return toInt(extraireDonnees().get("QUESTION"));
    }

    public void genererTrame(int numeroQuestion, int numeroReponse, int tempsReponse)
    {
        super.genererTrame(numeroQuestion + "", numeroReponse + "", tempsReponse + "");
    }
}
