package fr.hillionj.quizzy.ihm.widgets;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.List;

import fr.hillionj.quizzy.R;
import fr.hillionj.quizzy.communication.Peripherique;
import fr.hillionj.quizzy.parametres.Parametres;
import fr.hillionj.quizzy.parametres.Participant;

public class ListViewPeripheriques extends BaseAdapter {

    private Context context;
    private List<Peripherique> peripheriques = Parametres.getParametres().getPeripheriques();
    private ListView liste;

    public ListViewPeripheriques(AppCompatActivity activity, int id) {
        this.context = activity.getApplicationContext();
        liste = activity.findViewById(id);
        liste.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Peripherique peripherique = peripheriques.get(i);
                if (peripherique.estConnecte()) {
                    peripherique.deconnecter();
                } else {
                    peripherique.connecter();
                }
            }
        });
        mettreAjour();
    }

    @Override
    public int getCount() {
        return peripheriques.size();
    }

    @Override
    public Object[] getItem(int position) {
        Peripherique peripherique  =peripheriques.get(position);
        String description = "Non associé";
        Participant participantAssocier = Parametres.getParametres().getParticipantAssocier(peripherique);
        if (participantAssocier != null) {
            description = participantAssocier.getNom();
        }
        int couleur = Color.RED;
        if (peripherique.estConnecte()) {
            couleur = Color.GREEN;
        } else if (peripherique.seConnecte()) {
            couleur = Color.YELLOW;
        }
        return new Object[] {peripherique.getNom(), description, couleur};
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = LayoutInflater.from(context).inflate(R.layout.list_items, parent, false);
        }

        Object[] objet = getItem(position);

        String nom = (String) objet[0];
        String description = (String) objet[1];
        int couleur = (int) objet[2];

        TextView text1 = convertView.findViewById(R.id.text1);
        TextView text2 = convertView.findViewById(R.id.text2);
        View colorView = convertView.findViewById(R.id.couleur);

        text1.setText(nom);
        text2.setText(description);
        colorView.setBackgroundColor(couleur);

        return convertView;
    }

    public void mettreAjour() {
        liste.setAdapter(this);
    }
}
