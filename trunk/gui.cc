
struct colour   {
                    int red ;
                    int green ;
                    int blue ;
                } ;

colour bg = { 34, 139, 34 } ;

extern "C" gboolean handler_delete_event ( GtkWidget * widget, GdkEvent * event,
                                           gpointer user_data)  
{
    gtk_main_quit() ;
    cout<<"programma terminato regolarmente\n" ;
    return TRUE ;
}

extern "C" gboolean draw_play_ground ( GtkWidget * widget, cairo_t * cr,
                                       gpointer user_data )
{

   cairo_set_source_rgb(cr, bg.red, bg.green, bg.blue ) ;
   cairo_paint(cr);
     

   cairo_set_source_rgb(cr, 0.42, 0.65, 0.80);

   cairo_set_line_width(cr,6);

   cairo_rectangle (cr, 3, 3, 100, 100);

      color so it's actually visible 
   cairo_stroke(cr);


   cairo_set_source_rgb(cr, 0.17, 0.63, 0.12);
   cairo_set_line_width(cr,2);
   cairo_arc(cr, 150, 210, 20, 0, 2*G_PI);
   cairo_stroke(cr);


   cairo_set_source_rgb(cr, 0.77, 0.16, 0.13);
   cairo_set_line_width(cr, 6);
   cairo_move_to(cr, 80,160);
   cairo_line_to(cr, 200, 160);
   cairo_stroke(cr);

   return FALSE;

}

extern "C" gboolean handler_button_pressed_event ( GtkWidget * widget,
                                                   GdkEvent * event,
                                                   gpointer user_data )
{
    /*
     * widget       -> the object which received the signal
     */
    if      ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "new" )))
    {    cerr<<"new pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "mix" )))
    {    cerr<<"mix pressed"<<endl ; 




    }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "undo" )))
    {    cerr<<"undo pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "redo" )))
    {    cerr<<"redo pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "load" )))
    {    cerr<<"load pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "save" )))
    {    cerr<<"save pressed"<<endl ; }
    else if ( widget == GTK_WIDGET(gtk_builder_get_object( builder, "exit" )))
    {    cerr<<"exit pressed"<<endl ; }

    return TRUE ;
}
