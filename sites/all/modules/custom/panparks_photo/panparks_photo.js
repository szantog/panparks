(function ($) {

// We don't need behaviros if using jquery live.
//  Drupal.behaviors.panparks_photo = {
//    attach: function (context, settings) {
  $(function(){

      $('#popup').live('mouseenter', function() {

        //@todo this is ganyolasajavabol. I haven't jquery skill, I don't know what do this and why, but must do it
        //This is necessary to render pagers in colorbox. Need to hide the item counter, because the starting item is always the last
        //It seems, this is independent slideshow, therefore need to play with attribute rel later

        Drupal.settings.colorbox.current = '';
        Drupal.settings.colorbox.transition = 'none';
        $('#popup a')
          .filter('.colorbox')
          .once('init-colorbox-processed')
          .colorbox(Drupal.settings.colorbox);
      });

      $('#popup .colorbox').live('mouseenter', function() {
        // Unbind click event to keep off multiple click event.
        $(this).unbind("click");

        $(this).click(function () {
          if (!$.isFunction($.colorbox)) {
            return;
          }
          //This need to prevent double starting item in colorbox
          $(this).attr('rel', 'nofollow');

          $.colorbox({
            href: $(this).attr('href'),
            opacity: Drupal.settings.colorbox['opacity'],
            current: '',
            previous: Drupal.settings.colorbox['previous'],
            next: Drupal.settings.colorbox['next'],
            close: Drupal.settings.colorbox['close'],
            maxWidth: Drupal.settings.colorbox['maxWidth'],
            maxHeight: Drupal.settings.colorbox['maxHeight'],
            transition: 'none',
            __drupal_alter_by_ref: Drupal.settings.colorbox['__drupal_alter_by_ref'],
            rel: 'gallery'
          });
          //Add the previous removed attribute back, because after colorbox closed, this item can't render in slideshow
          $(this).attr('rel', 'gallery');
          return false;
        });
      });

      $('#popup .openlayers-tooltip-description').live('mouseenter', function() {
        // Original code: ajaxView.js
        if (Drupal.settings && Drupal.settings.views && Drupal.settings.views.ajaxViews) {
          // Retrieve the path to use for views' ajax.
          var ajax_path = Drupal.settings.views.ajax_path;
          // If there are multiple views this might've ended up showing up multiple times.
          if (ajax_path.constructor.toString().indexOf("Array") != -1) {
            ajax_path = ajax_path[0];
          }
          $.each(Drupal.settings.views.ajaxViews, function(i, settings) {
            var view = '.view-dom-id-' + settings.view_dom_id;
            var element_settings = {
              url: ajax_path,
              submit: settings,
              setClick: true,
              event: 'click',
              selector: view,
              progress: { type: 'throbber' }
            };
            // Process exposed filter forms.
            $('form#views-exposed-form-' + settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-'))
              .filter(':not(.views-processed)')
              .each(function () {
                var button = $('input[type=submit], input[type=image]', this);
                button = button[0];
                var ajax = new Drupal.ajax($(button).attr('id'), button, element_settings);
              })
              .addClass('views-processed')

            $(view).filter(':not(.views-processed)')
            // Don't attach to nested views. Doing so would attach multiple behaviors
            // to a given element.
// remove filter: bad working (openlayers views in parent)
//              .filter(function() {
//                // If there is at least one parent with a view class, this view
//                // is nested (e.g., an attachment). Bail.
//                return !$(this).parents('.view').size();
//              })
              .each(function() {
                // Set a reference that will work in subsequent calls.
                var target = this;
                $(this)
                  .addClass('views-processed')
                  // Process pager, tablesort, and attachment summary links.
                  .find('ul.pager > li > a, th.views-field a, .attachment .views-summary a')
                  .each(function () {
                    var viewData = {};
                    // Construct an object using the settings defaults and then overriding
                    // with data specific to the link.
                    $.extend(
                      viewData,
                      settings,
                      Drupal.Views.parseQueryString($(this).attr('href')),
                      // Extract argument data from the URL.
                      Drupal.Views.parseViewArgs($(this).attr('href'), settings.view_base_path)
                    );
                    // For anchor tags, these will go to the target of the anchor rather
                    // than the usual location.
                    $.extend(viewData, Drupal.Views.parseViewArgs($(this).attr('href'), settings.view_base_path));
                    element_settings.submit = viewData;
                    var ajax = new Drupal.ajax(false, this, element_settings);
                  }); // .each function () {
              }); // $view.filter().each
          }); // .each Drupal.settings.views.ajaxViews
        } // if
      });
   });

// Unused behaviro end.
//    }
//  };
}(jQuery));
