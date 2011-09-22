(function($) {
  Drupal.behaviors.maxlength_js = {
    attach: function(context, settings) {
      $('.maxlength-js', context).once('maxlength-js', function() {
        $(this).charCount();
      });
    },
    detach: function(context, settings) {
      $('.maxlength-js', context).removeOnce('maxlength-js', function() {
        $(this).charCount({
          action: 'detach'
        });
      });
    }
  };

  /**
   * Code below is based on:
   *   Character Count Plugin - jQuery plugin
   *   Dynamic character count for text areas and input fields
   *   written by Alen Grakalic
   *   http://cssglobe.com/post/7161/jquery-plugin-simplest-twitterlike-dynamic-character-count-for-textareas
   */

  $.fn.charCount = function(options) {
    // default configuration properties
    var defaults = {
      warning: 25,
      css: 'counter',
      counterElement: 'span',
      cssWarning: 'warning',
      cssExceeded: 'exceeded',
      counterText: Drupal.t('Characters left: '),
      action: 'attach'
    };

    var options = $.extend(defaults, options);

    if (options.action == 'detach') {
      this.each(function() {
        $(this).removeClass('maxlength-js-processed');
        $('#' + $(this).attr('id') + '-' + options.css).remove();
      });
      return 'removed';
    }

    function calculate(o) {
      var obj = $(o);
      var counter = $('#' + obj.attr('id') + '-' + options.css);
      var limit = parseInt(obj.attr('maxlength'));
      var count = obj.val().length;
      var available = limit - count;

      if (available <= options.warning && available >= 0) {
        counter.addClass(options.cssWarning);
      }
      else {
        counter.removeClass(options.cssWarning);
      }

      if (available < 0) {
        counter.addClass(options.cssExceeded);
        // Trim text
        obj.val(obj.val().substr(0, limit));
        // Re calculate text length
        count = obj.val().length;
        available = limit - count;
      }
      else {
        counter.removeClass(options.cssExceeded);
      }

      counter.html(options.counterText + available);
    };

    this.each(function() {
      $(this).after('<' + options.counterElement + ' id="' + $(this).attr('id') + '-' + options.css + '" class="' + options.css + '"></' + options.counterElement + '>');
      calculate(this);
      $(this).keyup(function() {
        calculate(this);
      });
      $(this).change(function() {
        calculate(this);
      });
    });

  };

})(jQuery);
