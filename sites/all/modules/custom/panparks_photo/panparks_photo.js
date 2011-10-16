/**
 *  @file
 */

(function ($) {

  Drupal.behaviors.panparks_photo = {
    attach: function (context, settings) {

      $('#popup .colorbox').live('mouseenter', function() {
        $(this).click(function () {
          if (!$.isFunction($.colorbox)) {
            return;
          }

          $.colorbox({
            href: $(this).attr('href'),
            opacity: settings.colorbox['opacity'],
            current: settings.colorbox['current'],
            previous: settings.colorbox['previous'],
            next: settings.colorbox['next'],
            close: settings.colorbox['close'],
            maxWidth: settings.colorbox['maxWidth'],
            maxHeight: settings.colorbox['maxHeight'],
            __drupal_alter_by_ref: settings.colorbox['__drupal_alter_by_ref']
          });
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
    }
  };

}(jQuery));
